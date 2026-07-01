#include "anilex/ui/designer/DesignerDragableLabel.h"
#include <QEnterEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QJsonDocument>

DesignerDragableLabel::DesignerDragableLabel(const QString &text, DesignerItem item, QWidget *parent)
  : QLabel(text, parent), m_properties(std::move(item)) {
  this->setFrameShape(QFrame::Box);
  this->setMargin(5);
}

void DesignerDragableLabel::enterEvent(QEnterEvent *event) {
  this->setCursor(Qt::OpenHandCursor);
  QLabel::enterEvent(event);
}

void DesignerDragableLabel::leaveEvent(QEvent *event) {
  this->unsetCursor();
  QLabel::leaveEvent(event);
}

void DesignerDragableLabel::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    m_dragStartPos = event->position();
    this->setCursor(Qt::ClosedHandCursor);
  }
  QLabel::mousePressEvent(event);
}

void DesignerDragableLabel::mouseMoveEvent(QMouseEvent *event) {
  if (!(event->buttons() & Qt::LeftButton)) return;

  if ((event->position() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance()) return;

  QDrag *drag = new QDrag(this);
  QMimeData *mimeData = new QMimeData();

  const QJsonObject json = designerItemToJsonObject(&m_properties);
  const QJsonDocument doc(json);
  const QByteArray data = doc.toJson(QJsonDocument::Compact);

  mimeData->setData("anilex/designer-data", data);
  drag->setMimeData(mimeData);

  drag->setPixmap(this->grab());
  drag->setHotSpot(event->position().toPoint());
  drag->exec(Qt::CopyAction);

  if (this->underMouse()) {
    this->setCursor(Qt::OpenHandCursor);
  } else {
    this->unsetCursor();
  }

  QLabel::mouseMoveEvent(event);
}
