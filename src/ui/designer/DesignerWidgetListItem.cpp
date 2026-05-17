#include "anilex/ui/designer/DesignerWidgetListItem.h"

#include <QApplication>
#include <QMimeData>
#include <QDrag>

#include "anilex/core/EnumConverter.h"
#include "anilex/ui/designer/DesignerItemFactory.h"

DesignerWidgetListItem::DesignerWidgetListItem(const QString &text, DesignerType::DesignerItemType type, QWidget *parent)
  : QLabel(text, parent), m_designerType(type) {
  this->setFrameShape(QFrame::Box);
  this->setMargin(5);
}

void DesignerWidgetListItem::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    m_dragStartPos = event->pos();
    this->setCursor(Qt::ClosedHandCursor);
  }
  QLabel::mousePressEvent(event);
}

void DesignerWidgetListItem::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    this->setCursor(Qt::OpenHandCursor);
  }
  QLabel::mouseReleaseEvent(event);
}

void DesignerWidgetListItem::mouseMoveEvent(QMouseEvent *event) {
  if (!(event->buttons() & Qt::LeftButton)) return;

  if ((event->pos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance()) return;

  QDrag *drag = new QDrag(this);
  QMimeData *mimeData = new QMimeData();

  qInfo() << m_designerType;
  QString type = EnumConverter::toString(m_designerType);
  qInfo() << type;
  mimeData->setData("application/x-designer-item", QByteArray(type.toUtf8()));
  drag->setMimeData(mimeData);

  drag->setPixmap(this->grab());
  drag->setHotSpot(event->pos());
  drag->exec(Qt::CopyAction);

  if (this->underMouse()) {
    this->setCursor(Qt::OpenHandCursor);
  } else {
    this->unsetCursor();
  }
}

void DesignerWidgetListItem::enterEvent(QEnterEvent *event) {
  this->setCursor(Qt::OpenHandCursor);
  QLabel::enterEvent(event);
}

void DesignerWidgetListItem::leaveEvent(QEvent *event) {
  this->unsetCursor();
  QLabel::leaveEvent(event);
}
