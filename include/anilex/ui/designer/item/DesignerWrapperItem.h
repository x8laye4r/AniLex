#pragma once
#include "anilex/ui/interfaces/AbstractDesignerItem.h"

class ItemSignalProxy : public QObject {
  Q_OBJECT
signals:
  void resizedItem(const QRectF &resizedRect);
};

class DesignerWrapperItem : public QGraphicsRectItem {
public:
  explicit DesignerWrapperItem(QGraphicsItem *parent = nullptr);
  ~DesignerWrapperItem() override;

  QRectF boundingRect() const override;

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  ItemSignalProxy *m_signal;
protected:
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  QPainterPath shape() const override;
private:
  enum class ResizeDirection { None, TopLeft, TopRight, BottomLeft, BottomRight };

  static constexpr qreal m_resizeButtonSize = 10.0;
  QPointF m_startDragPosition;
  ResizeDirection m_resizeDirection = ResizeDirection::None;
  QRectF m_startRect;

  ResizeDirection getResizeDirection(QPointF clickPos) const;
  QRectF getResizeButtonRect(ResizeDirection direction) const;
};
