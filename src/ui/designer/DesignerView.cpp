#include "anilex/ui/designer/DesignerView.h"

#include <QSizePolicy>

namespace {
  constexpr qreal kAspectRatio = 4.0 / 2.5;
}

DesignerView::DesignerView(QWidget *parent)
  : QGraphicsView(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  setAlignment(Qt::AlignCenter);
  setRenderHint(QPainter::Antialiasing);
  setDragMode(QGraphicsView::ScrollHandDrag);

  m_scene = new QGraphicsScene(this);
  this->setScene(m_scene);
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
