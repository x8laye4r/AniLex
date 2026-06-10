#pragma once
#include "anilex/ui/interfaces/AbstractDesignerItem.h"

class ItemSignalProxy : public QObject {
  Q_OBJECT
signals:
  void resizedItem(const QRectF &resizedRect);
  void resizeFinished(const QRectF &finalRect);
  void movedItem();
  void moveFinished();
};

class DesignerWrapperItem : public QGraphicsRectItem {
public:
  explicit DesignerWrapperItem(QGraphicsItem *parent = nullptr);
  ~DesignerWrapperItem() override;

  QRectF boundingRect() const override;

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void setWrappedItem(AbstractDesignerItem *wrappedItem);

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
  QPointF m_startPos;
  ResizeDirection m_resizeDirection = ResizeDirection::None;
  QRectF m_startRect;
  QRectF m_pendingRect;

  AbstractDesignerItem *m_wrappedItem = nullptr;

  ResizeDirection getResizeDirection(QPointF clickPos) const;
  QRectF getResizeButtonRect(ResizeDirection direction) const;
};
