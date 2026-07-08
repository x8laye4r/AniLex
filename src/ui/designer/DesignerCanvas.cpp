#include "anilex/ui/designer/DesignerCanvas.h"
#include <QMimeData>
#include <QMouseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>

#include "anilex/ui/designer/DesignerItemFactory.h"
#include "anilex/utils/EnumConverter.h"

namespace DesignerHelpers = anilex_designer_helpers;

DesignerCanvas::DesignerCanvas(QWidget *parent)
  : QWidget(parent) {
  this->setAcceptDrops(true);
  this->setMouseTracking(true);
}

void DesignerCanvas::mousePressEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton) return;
  if (!m_rubberBand) m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

  this->unselectAllItems();

  m_rubberBandStartPos = event->position();
  m_rubberBand->setGeometry(QRect(m_rubberBandStartPos.toPoint(), QSize()));
  m_rubberBand->show();

  update();
}

void DesignerCanvas::mouseReleaseEvent(QMouseEvent *event) {
  if (!m_rubberBand || !m_rubberBand->isVisible()) return;
  m_rubberBand->hide();
  QWidget::mouseReleaseEvent(event);
}

void DesignerCanvas::unselectAllItems() {
  for (IDesignerItem *item : m_selectedItems) {
    item->setSelected(false);
    QWidget *widget = item->asWidget();
    widget->update();
    widget->raise();
  }
  update();
  m_selectedItems.clear();
}

void DesignerCanvas::handleItemMousePress(IDesignerItem *item, QWidget *widget, const QMouseEvent *event) {
  const IDesignerItem::ResizeHandle handle = resizeHandleForObject(item->asWidget(), event->globalPosition().toPoint());

  if (!m_selectedItems.contains(item) || handle != IDesignerItem::ResizeHandle::None) {
    this->unselectAllItems();
    m_selectedItems.insert(item);
    item->setSelected(true);
  }

  if (handle != IDesignerItem::ResizeHandle::None && widget) {
    m_isResizing = true;
    m_resizeHandle = handle;
    m_resizeWidget = widget;
    m_resizeStartGeometry = widget->geometry();
    m_dragStartPos = event->globalPosition();
    // the resize cursor is already set by the item while hovering the handle
  } else {
    m_isDragging = true;
    m_dragStartPos = event->globalPosition();
    m_dragStartPositions.clear();
    for (IDesignerItem *selected : m_selectedItems) {
      m_dragStartPositions.insert(selected, selected->asWidget()->pos());
      selected->setDraggingCursor(true); // grabbing cursor while moving
    }
  }
  this->update();
}

void DesignerCanvas::handleItemMouseMove(QMouseEvent *event) {
  const QPoint delta = (event->globalPosition() - m_dragStartPos).toPoint();

  if (m_isResizing && (event->buttons() & Qt::LeftButton) && m_resizeWidget) {
    m_resizeWidget->setGeometry(resizedGeometry(m_resizeStartGeometry, m_resizeHandle, delta));
    update();
    return;
  }

  if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
    for (auto it = m_dragStartPositions.constBegin(); it != m_dragStartPositions.constEnd(); ++it) {
      QWidget *draggedWidget = it.key()->asWidget();
      draggedWidget->move(it.value() + delta);
      draggedWidget->raise();
    }
    update();
  }
}

void DesignerCanvas::handleItemMouseRelease() {
  // clear drag and pos because you stopped dragging
  m_isDragging = false;
  m_dragStartPositions.clear();

  // same for resizing
  m_isResizing = false;
  m_resizeHandle = IDesignerItem::ResizeHandle::None;
  m_resizeWidget = nullptr;

  // let each item revert its cursor back to the hover state
  for (IDesignerItem *item : m_selectedItems) {
    item->setDraggingCursor(false);
  }
}

IDesignerItem * DesignerCanvas::createItemFromJson(const QJsonObject &json) {
  const QString type = json.value("type").toString("NONE");
  std::unique_ptr<IDesignerItem> item = DesignerItemFactory::instance().getItem(type, this);
  if (!item) {
    qInfo() << Q_FUNC_INFO << "Type is not available" << type;
    return nullptr;
  }

  IDesignerItem *designerItem = item.release();
  designerItem->setType(EnumConverter::convertTo<DesignerHelpers::DesignerHelpers>(type));
  designerItem->installEventFilters_(this);
  designerItem->setGraphqlQuery(json.value("graphQL").toString(""));
  designerItem->setGraphqlSource(json.value("dataSource").toString(""));

  return designerItem;
}

void DesignerCanvas::selectItemsUnderRubberBand() {
  this->unselectAllItems();

  QRect rubberBandRect = m_rubberBand->geometry();

  for (IDesignerItem *item : m_items) {
    QWidget *widget = item->asWidget();
    if (rubberBandRect.intersects(widget->geometry())) {
      m_selectedItems.insert(item);
      item->setSelected(true);
      widget->raise();
      widget->update();
    }
  }
}

void DesignerCanvas::mouseMoveEvent(QMouseEvent *event) {
  if (!m_rubberBand || !m_rubberBand->isVisible()) return;

  const QRect newRect = QRect(m_rubberBandStartPos.toPoint(), event->position().toPoint()).normalized();
  m_rubberBand->setGeometry(newRect);

  selectItemsUnderRubberBand();

  QWidget::mouseMoveEvent(event);
}

IDesignerItem *DesignerCanvas::itemForObject(const QObject *watched) const {
  for (IDesignerItem *item : m_items) {
    if (item->asWidget() == watched) return item;
  }
  return nullptr;
}

IDesignerItem::ResizeHandle DesignerCanvas::resizeHandleForObject(const QObject *watched, QPoint globalPos) const {
  IDesignerItem *item = itemForObject(watched);
  if (!item) return IDesignerItem::ResizeHandle::None;

  return item->resizeHandleAt(item->asWidget()->mapFromGlobal(globalPos));
}

QRect DesignerCanvas::resizedGeometry(const QRect &start, IDesignerItem::ResizeHandle handle, QPoint delta) {
  constexpr int minW = kMinItemSize.width();
  constexpr int minH = kMinItemSize.height();
  QRect resizedRect = start;

  switch (handle) {
    case IDesignerItem::ResizeHandle::TopLeft:
      resizedRect.setLeft(qMin(start.left() + delta.x(), start.right() - minW + 1));
      resizedRect.setTop(qMin(start.top() + delta.y(), start.bottom() - minH + 1));
      break;
    case IDesignerItem::ResizeHandle::TopRight:
      resizedRect.setRight(qMax(start.right() + delta.x(), start.left() + minW - 1));
      resizedRect.setTop(qMin(start.top() + delta.y(), start.bottom() - minH + 1));
      break;
    case IDesignerItem::ResizeHandle::BottomLeft:
      resizedRect.setLeft(qMin(start.left() + delta.x(), start.right() - minW + 1));
      resizedRect.setBottom(qMax(start.bottom() + delta.y(), start.top() + minH - 1));
      break;
    case IDesignerItem::ResizeHandle::BottomRight:
      resizedRect.setRight(qMax(start.right() + delta.x(), start.left() + minW - 1));
      resizedRect.setBottom(qMax(start.bottom() + delta.y(), start.top() + minH - 1));
      break;
    default:
      break;
  }

  return resizedRect;
}

bool DesignerCanvas::eventFilter(QObject *watched, QEvent *event) {
  IDesignerItem *item = itemForObject(watched);
  if (!item) return QWidget::eventFilter(watched, event);

  QWidget *widget = item->asWidget();

  switch (event->type()) {
    case QEvent::MouseButtonPress: {
      QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
      if (mouseEvent->button() != Qt::LeftButton) break;
      this->handleItemMousePress(item, widget, mouseEvent);
      return true;
    }
    case QEvent::MouseMove: {
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
        if (m_isDragging || m_isResizing) {
          this->handleItemMouseMove(mouseEvent);
          return true;
        }
        return false;
    }
    case QEvent::MouseButtonRelease: {
      const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
      if (mouseEvent->button() != Qt::LeftButton) break;
      this->handleItemMouseRelease();
      return true;
    }
    default:
      break;
  }

  return QWidget::eventFilter(watched, event);
}

void DesignerCanvas::dropEvent(QDropEvent *event) {
  const QMimeData *mimeData = event->mimeData();
  if (mimeData->hasFormat("anilex/designer-data")) {
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(mimeData->data("anilex/designer-data"), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
      qInfo() << Q_FUNC_INFO << parseError.errorString();
      return;
    }

    if (IDesignerItem *designerItem = createItemFromJson(doc.object())) {
      QWidget *widget = designerItem->asWidget();
      widget->setMouseTracking(true);
      widget->move(event->position().toPoint());
      widget->show();
      m_items.append(designerItem);
    }
  }
  QWidget::dropEvent(event);
}

void DesignerCanvas::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasFormat("anilex/designer-data")) [[likely]] {
    event->acceptProposedAction();
  } else [[unlikely]] {
    event->ignore();
  }
  QWidget::dragEnterEvent(event);
}

void DesignerCanvas::dragMoveEvent(QDragMoveEvent *event) {
  event->acceptProposedAction();
  QWidget::dragMoveEvent(event);
}

void DesignerCanvas::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);

  const QRect rect = this->rect();
  painter.setBrush(QBrush(Qt::gray));
  painter.setPen(QPen(Qt::black, 5));

  painter.drawRoundedRect(rect.adjusted(1, 1, -1, -1), 10, 10);

  QWidget::paintEvent(event);
}
