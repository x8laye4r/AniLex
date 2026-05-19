#include "anilex/ui/designer/item/DesignerItemWrapper.h"
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsObject>

DesignerItemWrapper::DesignerItemWrapper(QGraphicsItem *parent)
  : QGraphicsObject(parent) {
  this->setFlags(ItemIsMovable | ItemIsSelectable);
  this->setAcceptHoverEvents(true);
}

void DesignerItemWrapper::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option); Q_UNUSED(widget);
  if (this->isSelected()) {
    painter->save();

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::white, 2, Qt::DashLine));

    const QRectF rect = this->boundingRect();
    painter->drawRect(rect);

    // draw corner rects to resize
    const qreal xLeft = rect.left() - 5;
    const qreal xRight = rect.right() - 5;
    const qreal yTop = rect.top() - 5;
    const qreal yBottom = rect.bottom() - 5;

    const QRectF rects[] = {
      QRectF(xLeft, yTop,    m_resizeButtonSize, m_resizeButtonSize), // bottom left
      QRectF(xLeft,yBottom, m_resizeButtonSize, m_resizeButtonSize), // top left
      QRectF(xRight,yTop,    m_resizeButtonSize, m_resizeButtonSize), // top right
      QRectF(xRight, yBottom, m_resizeButtonSize, m_resizeButtonSize)  // bottom right
    };

    painter->drawRects(rects, std::size(rects));
  }
}

void DesignerItemWrapper::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
  if (isSelected()) {
    ResizeDirection direction = getResizeDirection(event->pos());

    if (direction == TopLeft || direction == BottomLeft) {
      this->setCursor(Qt::SizeFDiagCursor);
    }
    else if (direction == TopRight || direction == BottomRight) {
      this->setCursor(Qt::PointingHandCursor);
    } else {
      this->setCursor(Qt::ArrowCursor);
    }
  } else {
    this->setCursor(Qt::ArrowCursor);
  }
  QGraphicsObject::hoverMoveEvent(event);
}

void DesignerItemWrapper::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    m_resizeDirection = getResizeDirection(event->pos());

    if (m_resizeDirection != None) {
      event->accept();
      m_startDragPosition = event->pos();
      return;
    }
  }
  QGraphicsObject::mousePressEvent(event);
}

void DesignerItemWrapper::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton && m_resizeDirection != None) {
    this->prepareGeometryChange();

    QPointF delta = event->pos() - m_startDragPosition;
    QRectF beforeRect = this->boundingRect();
    QRectF resizedRect = this->boundingRect();

    switch (m_resizeDirection) {
      case TopLeft:
        resizedRect.setTopLeft(beforeRect.topLeft() + delta);
        break;
      case TopRight:
        resizedRect.setTopRight(beforeRect.topRight() + delta);
        break;
      case BottomLeft:
        resizedRect.setBottomLeft(beforeRect.bottomLeft() + delta);
        break;
      case BottomRight:
        resizedRect.setBottomRight(beforeRect.bottomRight() + delta);
        break;
    }

    const qreal minHeight = 40.0;
    const qreal minWidth = 40.0;

    if (resizedRect.width() >= minWidth && resizedRect.height() >= minHeight) {
      // now put here the resize logic (don't know how currently but will be implemented then)
      update();
      emit elementResized();
    }

    event->accept();
  } else {
    QGraphicsObject::mouseMoveEvent(event);
  }
}

void DesignerItemWrapper::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsObject::mouseReleaseEvent(event);
}

DesignerItemWrapper::ResizeDirection DesignerItemWrapper::getResizeDirection(QPointF clickPos) const {
  const QRectF rect = this->boundingRect();
  const qreal xLeft = rect.left() - 5;
  const qreal xRight = rect.right() - 5;
  const qreal yTop = rect.top() - 5;
  const qreal yBottom = rect.bottom() - 5;

  if (QRectF(xLeft, yTop, m_resizeButtonSize, m_resizeButtonSize).contains(clickPos)) {
    return TopLeft;
  }
  if (QRectF(xLeft, yBottom, m_resizeButtonSize, m_resizeButtonSize).contains(clickPos)) {
    return BottomLeft;
  }
  if (QRectF(xRight, yTop, m_resizeButtonSize, m_resizeButtonSize).contains(clickPos)) {
    return TopRight;
  }
  if (QRectF(xRight, yBottom, m_resizeButtonSize, m_resizeButtonSize).contains(clickPos)) {
    return BottomRight;
  }

  return None;
}


