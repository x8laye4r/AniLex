#pragma once
#include "anilex/ui/interfaces/AbstractDesignerItem.h"

class QLabel;

class DesignerIconItem : public AbstractDesignerItem {
  Q_OBJECT

  Q_PROPERTY(QColor color READ color WRITE setColor DESIGNABLE true)
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor DESIGNABLE true)
  Q_PROPERTY(qreal borderRadius READ borderRadius WRITE setBorderRadius DESIGNABLE true)
  Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor DESIGNABLE true)
  Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth DESIGNABLE true)
public:
  DesignerIconItem();
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void updateGeometry() override;
  QRectF boundingRect() const override;

  QColor color() const;
  void setColor(const QColor &color);

  QColor backgroundColor() const;
  void setBackgroundColor(const QColor &backgroundColor);

  qreal borderRadius() const;
  void setBorderRadius(qreal borderRadius);

  QColor borderColor() const;
  void setBorderColor(const QColor &borderColor);

  qreal borderWidth() const;
  void setBorderWidth(qreal borderWidth);

  qreal padding() const;
  void setPadding(qreal padding);

private:
  QString m_text;
  QPixmap *m_pixmap = nullptr;
  QLabel *m_label = nullptr;

  QColor m_color = Qt::black;
  QColor m_backgroundColor = Qt::transparent;
  QColor m_borderColor = Qt::transparent;

  qreal m_borderRadius = 12.0f;
  qreal m_borderWidth = 0.0f;
  qreal m_padding = 5.0f;
};