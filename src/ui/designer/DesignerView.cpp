#include "anilex/ui/designer/DesignerView.h"

#include <QSizePolicy>
#include <QDropEvent>
#include <QMimeData>

#include "anilex/ui/designer/DesignerItemFactory.h"

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
  setDragMode(QGraphicsView::ScrollHandDrag);

  m_scene = new QGraphicsScene(this);
  this->setScene(m_scene);
  this->setAcceptDrops(true);
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

      newItem->setPos(scenePos);
      this->m_scene->addItem(newItem.release());
    } else {
      qInfo() << "Could not find designer-item";
    }
    event->acceptProposedAction();
  }else {
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
