#include "anilex/ui/designer/item/DesignerIconItem.h"
#include "anilex/ui/designer/DesignerItemFactory.h"

DesignerIconItem::DesignerIconItem() {
  m_designerType = DesignerType::DesignerItemType::BOOLEAN;
  m_pixmap = new QPixmap();
  m_pixmap->fill(Qt::transparent);
  m_label = new QLabel();
}

void DesignerIconItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option); Q_UNUSED(widget);
  painter->save();

  painter->setRenderHint(QPainter::Antialiasing);

  QRectF rect = this->boundingRect();

  if (m_backgroundColor.isValid() && m_backgroundColor != Qt::transparent) {
    painter->setBrush(QBrush(m_backgroundColor));
  } else {
    painter->setBrush(Qt::NoBrush);
  }

  if (m_borderColor.isValid() && m_borderColor != Qt::transparent) {
    QPen pen(m_borderColor);
    pen.setWidth(2);
    painter->setPen(pen);
  } else {
    painter->setPen(Qt::NoPen);
  }

  if (m_borderRadius > 0) {
    painter->drawRoundedRect(rect, m_borderRadius, m_borderRadius);
  } else {
    painter->drawRect(rect);
  }

  painter->setFont(m_label->font());
  painter->setPen(Qt::black);

  double currentX = m_padding;
  double centerY = rect.height() / 2.0;

  if (m_pixmap && !m_pixmap->isNull()) {
    double pixmapY = centerY - (m_pixmap->height() / 2.0);
    painter->drawPixmap(currentX, pixmapY, *m_pixmap);
    currentX += m_pixmap->width() + 8;
  }

  if (!m_labelText.isEmpty()) {
    QFontMetrics metrics(m_label->font());
    double textY = centerY + (metrics.ascent() - metrics.descent()) / 2.0;
    painter->drawText(currentX, textY, m_labelText);
  }

  painter->restore();

  AbstractDesignerItem::paint(painter, option, widget);
}

void DesignerIconItem::updateGeometry() {
  QFontMetrics metrics(m_label->font());

  const int pixmapWidth = m_pixmap->width();
  const int pixmapHeight = m_pixmap->height();

  const int textWidth = metrics.horizontalAdvance(m_labelText);
  const int textHeight = metrics.height();

  int spacing = pixmapWidth > 0 && textWidth > 0 ? 8 : 0;
  m_width = (m_padding * 2) + pixmapWidth + spacing + textWidth;
  m_height = (m_padding * 2) + qMax(pixmapHeight, textHeight);

  prepareGeometryChange();
}

QRectF DesignerIconItem::boundingRect() const {
  return QRectF(0, 0, m_width, m_height);
}

QColor DesignerIconItem::color() const {
  return m_color;
}

void DesignerIconItem::setColor(const QColor &color) {
  m_color = color;
}

QColor DesignerIconItem::backgroundColor() const {
  return m_backgroundColor;
}

void DesignerIconItem::setBackgroundColor(const QColor &color) {
  m_backgroundColor = color;
}

QColor DesignerIconItem::borderColor() const {
  return m_borderColor;
}

void DesignerIconItem::setBorderColor(const QColor &color) {
  m_borderColor = color;
}

qreal DesignerIconItem::borderRadius() const {
  return m_borderRadius;
}

void DesignerIconItem::setBorderRadius(qreal radius) {
  m_borderRadius = radius;
}

void DesignerIconItem::setText(const QString &text) {
  m_labelText = text;
}

REGISTER_DESIGNER_ITEM(DesignerIconItem, "BOOLEAN")
