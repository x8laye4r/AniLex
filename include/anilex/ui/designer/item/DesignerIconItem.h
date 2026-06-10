#pragma once
#include "anilex/ui/interfaces/AbstractDesignerItem.h"
#include <QLabel>
#include <QPixmap>

class DesignerIconItem : public AbstractDesignerItem {
  Q_OBJECT

  Q_PROPERTY(QColor color READ color WRITE setColor)
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
  Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
  Q_PROPERTY(qreal borderRadius READ borderRadius WRITE setBorderRadius)
public:
  DesignerIconItem();
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void updateGeometry() override;
  QRectF boundingRect() const override;

  QColor color() const;
  void setColor(const QColor &color);

  QColor backgroundColor() const;
  void setBackgroundColor(const QColor &color);

  QColor borderColor() const;
  void setBorderColor(const QColor &color);

  qreal borderRadius() const;
  void setBorderRadius(qreal radius);

  void setText(const QString &text);

private:
  QPixmap *m_pixmap = nullptr;
  QLabel *m_label = nullptr;
  QString m_labelText;


  QColor m_color;
  QColor m_backgroundColor = Qt::transparent;
  QColor m_borderColor = Qt::transparent;
  qreal m_borderRadius;
  qreal m_height;
  qreal m_width;
  qreal m_padding = 10;
};
