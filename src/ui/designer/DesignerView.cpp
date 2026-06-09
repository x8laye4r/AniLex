#include "anilex/ui/designer/DesignerView.h"

#include <QSizePolicy>
#include <QDropEvent>
#include <QMimeData>

#include "anilex/ui/designer/DesignerItemFactory.h"
#include "anilex/ui/designer/item/DesignerWrapperItem.h"

namespace {
  constexpr qreal kAspectRatio = 4.0 / 2.5;
}

/*
 * TODO: fix so you can move the items inside the View
 * fix: make a wrapper for the QGraphicsProxyWidget like a QGraphicsItem post:
 * https://stackoverflow.com/questions/15413564/make-qgraphicsproxywidget-movable-selectable
 */

DesignerView::DesignerView(QWidget *parent)
  : QGraphicsView(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  setAlignment(Qt::AlignCenter);
  setRenderHint(QPainter::Antialiasing);
  setDragMode(QGraphicsView::NoDrag);

  // set to no scrollbars
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  m_scene = new QGraphicsScene(this);
  this->setScene(m_scene);
  this->setAcceptDrops(true);

  // set so the view doesn't move and stays in one place
  this->setSceneRect(this->mapToScene(this->viewport()->rect()).boundingRect());
}

QSize DesignerView::sizeHint() const {
  return {480, 300};
}

QSize DesignerView::minimumSizeHint() const {
  return {180, 112};
}

bool DesignerView::hasHeightForWidth() const {
  return true;
}

int DesignerView::heightForWidth(int width) const {
  return static_cast<int>(width / kAspectRatio);
}

void DesignerView::dropEvent(QDropEvent *event) {
  if (event->mimeData()->hasFormat("application/x-designer-item")) {
    qInfo() << "Found application/x-designer-item";
    QByteArray data = event->mimeData()->data("application/x-designer-item");
    QString type = QString::fromUtf8(data);
    qInfo() << type;

    if (std::unique_ptr<AbstractDesignerItem> newItem = DesignerItemFactory::instance().getItem(type)) {
      QPoint viewPos = event->position().toPoint();
      QPointF scenePos = this->mapToScene(viewPos);

      DesignerWrapperItem *wrapperItem = new DesignerWrapperItem();
      wrapperItem->setRect(newItem->boundingRect());
      wrapperItem->setPos(scenePos);

      AbstractDesignerItem* rawItem = newItem.release();
      rawItem->setPos(0, 0);
      rawItem->setParentItem(wrapperItem);
      rawItem->setAcceptedMouseButtons(Qt::NoButton);
      rawItem->setFlag(QGraphicsItem::ItemIsMovable, false);
      rawItem->setFlag(QGraphicsItem::ItemIsSelectable, false);

      connect(wrapperItem->m_signal, &ItemSignalProxy::resizedItem, rawItem, &AbstractDesignerItem::resizeRect);

      connect(wrapperItem->m_signal, &ItemSignalProxy::movedItem, rawItem, [rawItem]() {
        emit rawItem->xChanged();
        emit rawItem->yChanged();
      });

      this->m_scene->addItem(wrapperItem);
    } else {
      qInfo() << "Could not find designer-item";
    }
    event->acceptProposedAction();
  } else {
    event->ignore();
  }
}

void DesignerView::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasFormat("application/x-designer-item")) {
    event->accept();
  } else {
    event->ignore();
  }
}

void DesignerView::dragMoveEvent(QDragMoveEvent *event) {
  if (event->mimeData()->hasFormat("application/x-designer-item")) {
    event->accept();
  } else {
    event->ignore();
  }
}

void DesignerView::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    if (const size_t count = m_scene->selectedItems().size(); count > 0) {
      event->accept();
      return;
    }
  }
  QGraphicsView::mousePressEvent(event);
}
