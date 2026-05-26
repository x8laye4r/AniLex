#pragma once
#include <QWidget>

/*
 * TODO: add alpha channel for the colors
 * TODO: rename functions to better names
 * TODO: maybe some more customization
*/

class CustomPixmap : public QWidget {
  Q_OBJECT
public:
  explicit CustomPixmap(QWidget* parent = nullptr);

  int cornerRadius() const;
  void setRoundedCorners(int radius);
  void setRoundedCorners(int topLeft, int topRight, int bottomRight, int bottomLeft);

  QColor overlayColor() const;
  QColor shadowColor() const;
  int shadowOffsetX() const;
  int shadowOffsetY() const;
  void setOverlay(const QColor &color);
  void setShadow(const QColor &color);
  void setOffsetShadow(const int offsetX, const int offsetY);

  QColor borderColor() const;
  short borderThickness() const;
  void setBorder(const QColor &color);
  void setBorderThickness(const short thickness);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  QPixmap *m_pixmap = nullptr;

  int m_radiusTopLeft = 0;
  int m_radiusTopRight = 0;
  int m_radiusBottomRight = 0;
  int m_radiusBottomLeft = 0;

  QColor m_overlayColor = Qt::transparent;
  QColor m_shadowColor = Qt::transparent;
  int m_shadowOffsetX = 0;
  int m_shadowOffsetY = 0;

  QColor m_borderColor = Qt::transparent;
  short m_borderThickness = 0;
};
