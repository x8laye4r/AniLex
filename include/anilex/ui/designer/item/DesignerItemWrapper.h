#pragma once
#include <QGraphicsObject>

class DesignerItemWrapper : public QGraphicsObject {
public:
  explicit DesignerItemWrapper(QGraphicsItem *parent = nullptr);
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
  void elementResized();
protected:
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
  enum ResizeDirection { None, TopLeft, TopRight, BottomLeft, BottomRight };

  ResizeDirection m_resizeDirection = None;
  const qreal m_resizeButtonSize = 10;
  QPointF m_startDragPosition;

  ResizeDirection getResizeDirection(QPointF clickPos) const;
};
