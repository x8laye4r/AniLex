#include "anilex/ui/designer/item/DesignerWrapperItem.h"
#include <QGraphicsSceneMouseEvent>

DesignerWrapperItem::DesignerWrapperItem(QGraphicsItem *parent)
  : QGraphicsRectItem(parent), m_signal(new ItemSignalProxy()) {
  this->setFlags(ItemIsMovable | ItemIsSelectable | ItemStacksBehindParent | ItemSendsGeometryChanges);
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
  const qreal tolerance = 5.0;

  auto hitBox = [tolerance](const QRectF& rect) {
    return rect.adjusted(-tolerance, -tolerance, tolerance, tolerance);
  };

  if (hitBox(getResizeButtonRect(ResizeDirection::TopLeft)).contains(clickPos)) {
    return ResizeDirection::TopLeft;
  }
  if (hitBox(getResizeButtonRect(ResizeDirection::TopRight)).contains(clickPos)) {
    return ResizeDirection::TopRight;
  }
  if (hitBox(getResizeButtonRect(ResizeDirection::BottomLeft)).contains(clickPos)) {
    return ResizeDirection::BottomLeft;
  }
  if (hitBox(getResizeButtonRect(ResizeDirection::BottomRight)).contains(clickPos)) {
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

    QRectF resizedRect = m_startRect;
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


    resizedRect = resizedRect.normalized();

    this->prepareGeometryChange();
    this->setRect(resizedRect);
    emit m_signal->resizedItem(resizedRect);
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
      m_startRect = this->rect();
      return;
    }
  }
  QGraphicsRectItem::mousePressEvent(event);
}

void DesignerWrapperItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsRectItem::mouseReleaseEvent(event);
}

QPainterPath DesignerWrapperItem::shape() const {
  QPainterPath path;
  path.addRect(this->rect());

  if (isSelected()) {
    const qreal tolerance = 5.0;
    auto hitBox = [tolerance](const QRectF& rect) {
      return rect.adjusted(-tolerance, -tolerance, tolerance, tolerance);
    };

    path.addRect(hitBox(getResizeButtonRect(ResizeDirection::TopLeft)));
    path.addRect(hitBox(getResizeButtonRect(ResizeDirection::TopRight)));
    path.addRect(hitBox(getResizeButtonRect(ResizeDirection::BottomLeft)));
    path.addRect(hitBox(getResizeButtonRect(ResizeDirection::BottomRight)));
  }

  return path;
}

QVariant DesignerWrapperItem::itemChange(GraphicsItemChange change, const QVariant &value) {
  if (change == ItemPositionHasChanged) {
    emit m_signal->movedItem();
  }
  return QGraphicsRectItem::itemChange(change, value);
}
