#include "anilex/ui/designer/item/DesignerWrapperItem.h"
#include <QGraphicsSceneMouseEvent>

DesignerWrapperItem::DesignerWrapperItem(QGraphicsItem *parent)
  : QGraphicsRectItem(parent), m_signal(new ItemSignalProxy()) {
  this->setFlags(ItemIsMovable | ItemIsSelectable);
  this->setAcceptHoverEvents(true);
}

DesignerWrapperItem::~DesignerWrapperItem() {
  delete m_signal;
}

QRectF DesignerWrapperItem::boundingRect() const {
  QRectF baseRect = QGraphicsRectItem::boundingRect();
  qreal padding = m_resizeButtonSize / 2.0;
  return baseRect.adjusted(-padding, -padding, padding, padding);
}

void DesignerWrapperItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option); Q_UNUSED(widget);
  
  if (this->isSelected()) {
    painter->save();

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::white, 2, Qt::DashLine));

    const QRectF boundingRect = this->boundingRect();
    painter->drawRect(boundingRect);

    // Draw resize handles at corners
    painter->fillRect(getResizeButtonRect(ResizeDirection::TopLeft), Qt::white);
    painter->fillRect(getResizeButtonRect(ResizeDirection::TopRight), Qt::white);
    painter->fillRect(getResizeButtonRect(ResizeDirection::BottomLeft), Qt::white);
    painter->fillRect(getResizeButtonRect(ResizeDirection::BottomRight), Qt::white);

    painter->restore();
  }
}

QRectF DesignerWrapperItem::getResizeButtonRect(ResizeDirection direction) const {
  const QRectF rect = this->rect();
  const qreal buttonSize = m_resizeButtonSize;
  
  switch (direction) {
    case ResizeDirection::TopLeft:
      return QRectF(rect.left() - buttonSize / 2, rect.top() - buttonSize / 2, buttonSize, buttonSize);
    case ResizeDirection::TopRight:
      return QRectF(rect.right() - buttonSize / 2, rect.top() - buttonSize / 2, buttonSize, buttonSize);
    case ResizeDirection::BottomLeft:
      return QRectF(rect.left() - buttonSize / 2, rect.bottom() - buttonSize / 2, buttonSize, buttonSize);
    case ResizeDirection::BottomRight:
      return QRectF(rect.right() - buttonSize / 2, rect.bottom() - buttonSize / 2, buttonSize, buttonSize);
  }
}

DesignerWrapperItem::ResizeDirection DesignerWrapperItem::getResizeDirection(QPointF clickPos) const {
  if (getResizeButtonRect(ResizeDirection::TopLeft).contains(clickPos)) {
    return ResizeDirection::TopLeft;
  }
  if (getResizeButtonRect(ResizeDirection::TopRight).contains(clickPos)) {
    return ResizeDirection::TopRight;
  }
  if (getResizeButtonRect(ResizeDirection::BottomLeft).contains(clickPos)) {
    return ResizeDirection::BottomLeft;
  }
  if (getResizeButtonRect(ResizeDirection::BottomRight).contains(clickPos)) {
    return ResizeDirection::BottomRight;
  }

  return ResizeDirection::None;
}

void DesignerWrapperItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
  if (isSelected()) {
    ResizeDirection direction = getResizeDirection(event->pos());

    if (direction == ResizeDirection::TopLeft || direction == ResizeDirection::BottomRight) {
      this->setCursor(Qt::SizeFDiagCursor);
    }
    else if (direction == ResizeDirection::TopRight || direction == ResizeDirection::BottomLeft) {
      this->setCursor(Qt::SizeBDiagCursor);
    } else {
      this->setCursor(Qt::ArrowCursor);
    }
  } else {
    this->setCursor(Qt::ArrowCursor);
  }
  QGraphicsRectItem::hoverMoveEvent(event);
}

void DesignerWrapperItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton && m_resizeDirection != ResizeDirection::None) {
    QPointF delta = event->pos() - m_startDragPosition;

    QRectF resizedRect = this->rect();
    switch (m_resizeDirection) {
      case ResizeDirection::TopLeft:
        resizedRect.setTopLeft(resizedRect.topLeft() + delta);
        break;
      case ResizeDirection::TopRight:
        resizedRect.setTopRight(resizedRect.topRight() + delta);
        break;
      case ResizeDirection::BottomLeft:
        resizedRect.setBottomLeft(resizedRect.bottomLeft() + delta);
        break;
      case ResizeDirection::BottomRight:
        resizedRect.setBottomRight(resizedRect.bottomRight() + delta);
        break;
      case ResizeDirection::None:
        break;
    }

    constexpr qreal minHeight = 40.0;
    constexpr qreal minWidth = 40.0;

    if (resizedRect.width() >= minWidth && resizedRect.height() >= minHeight) {
      this->prepareGeometryChange();
      this->setRect(resizedRect);
      m_signal->resizedItem(resizedRect);
    }
    event->accept();
  } else {
    QGraphicsRectItem::mouseMoveEvent(event);
  }
}

void DesignerWrapperItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    m_resizeDirection = getResizeDirection(event->pos());

    if (m_resizeDirection != ResizeDirection::None) {
      event->accept();
      m_startDragPosition = event->pos();
      return;
    }
  }
  QGraphicsRectItem::mousePressEvent(event);
}

void DesignerWrapperItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsRectItem::mouseReleaseEvent(event);
}
