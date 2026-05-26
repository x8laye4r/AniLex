#include "anilex/ui/CustomPixmap.h"

#include <QPainter>
#include <qpainterpath.h>

CustomPixmap::CustomPixmap(QWidget *parent)
  : QWidget(parent) {
  m_pixmap = new QPixmap(300, 200);
  m_pixmap->fill(Qt::white);

  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

int CustomPixmap::cornerRadius() const {
  return m_radiusTopRight; // Assuming all corners have the same radius, return one of them
}

QColor CustomPixmap::overlayColor() const {
  return m_overlayColor;
}

QColor CustomPixmap::shadowColor() const {
  return m_shadowColor;
}

int CustomPixmap::shadowOffsetX() const {
  return m_shadowOffsetX;
}

int CustomPixmap::shadowOffsetY() const {
  return m_shadowOffsetY;
}

QColor CustomPixmap::borderColor() const {
  return m_borderColor;
}

short CustomPixmap::borderThickness() const {
  return m_borderThickness;
}

void CustomPixmap::setRoundedCorners(const int radius) {
  setRoundedCorners(radius, radius, radius, radius);
}

void CustomPixmap::setRoundedCorners(const int topLeft, const int topRight, const int bottomRight, const int bottomLeft) {
  m_radiusTopLeft = topLeft;
  m_radiusTopRight = topRight;
  m_radiusBottomRight = bottomRight;
  m_radiusBottomLeft = bottomLeft;
  update();
}

void CustomPixmap::setOverlay(const QColor &color) {
  m_overlayColor = color;
  update();
}

void CustomPixmap::setShadow(const QColor &color) {
  m_shadowColor = color;
  update();
}

void CustomPixmap::setOffsetShadow(const int offsetX, const int offsetY) {
  m_shadowOffsetX = offsetX;
  m_shadowOffsetY = offsetY;
  update();
}

void CustomPixmap::setBorder(const QColor &color) {
  m_borderColor = color;
  update();
}

void CustomPixmap::setBorderThickness(const short thickness) {
  m_borderThickness = thickness;
  update();
}

void CustomPixmap::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

  QRectF contentRect = this->rect();
  if (m_shadowColor != Qt::transparent) {
    contentRect.adjust(0, 0, -qAbs(m_shadowOffsetX), -qAbs(m_shadowOffsetY));
  }

  QPainterPath path;
  path.setFillRule(Qt::WindingFill);

  path.moveTo(contentRect.topLeft() + QPointF(m_radiusTopLeft, 0));
  path.lineTo(contentRect.topRight() - QPointF(m_radiusTopRight, 0));
  path.quadTo(contentRect.topRight(), contentRect.topRight() + QPointF(0, m_radiusTopRight));
  path.lineTo(contentRect.bottomRight() - QPointF(0, m_radiusBottomRight));
  path.quadTo(contentRect.bottomRight(), contentRect.bottomRight() - QPointF(m_radiusBottomRight, 0));
  path.lineTo(contentRect.bottomLeft() + QPointF(m_radiusBottomLeft, 0));
  path.quadTo(contentRect.bottomLeft(), contentRect.bottomLeft() - QPointF(0, m_radiusBottomLeft));
  path.lineTo(contentRect.topLeft() + QPointF(0, m_radiusTopLeft));
  path.quadTo(contentRect.topLeft(), contentRect.topLeft() + QPointF(m_radiusTopLeft, 0));

  // paint shadow
  if (m_shadowColor != Qt::transparent) {
    painter.save();
    painter.translate(m_shadowOffsetX, m_shadowOffsetY);
    painter.fillPath(path, m_shadowColor);
    painter.restore();
  }

  // paint border
  if (m_borderColor != Qt::transparent && m_borderThickness > 0) {
    painter.save();
    QPen pen(m_borderColor, m_borderThickness);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawPath(path);
    painter.restore();

    qreal halfThickness = m_borderThickness / 2.0;
    contentRect.adjust(halfThickness, halfThickness, -halfThickness, -halfThickness);
  }

  painter.save();
  painter.setClipPath(path);

  if (m_pixmap && !m_pixmap->isNull()) {
    painter.drawPixmap(contentRect.toRect(), *m_pixmap);
  } else {
    painter.fillRect(contentRect.toRect(), QColor(240, 240, 240));
  }

  if (m_overlayColor != Qt::transparent) {
    painter.fillRect(contentRect.toRect(), m_overlayColor);
  }

  painter.restore();
}
