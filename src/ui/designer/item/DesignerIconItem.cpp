#include "anilex/ui/designer/item/DesignerIconItem.h"
#include "anilex/ui/designer/DesignerItemFactory.h"

#include <QLabel>
#include <QPixmap>

DesignerIconItem::DesignerIconItem() {
  m_designerType = DesignerType::DesignerItemType::BOOLEAN;
  m_text = "Icon";

  m_label = new QLabel();
  m_label->setAttribute(Qt::WA_TranslucentBackground);
  m_label->setAlignment(Qt::AlignCenter);
  m_label->setText(m_text);

  this->setWidget(m_label);

  m_pixmap = new QPixmap();
  m_pixmap->fill(Qt::transparent);

  this->DesignerIconItem::updateGeometry();
}

/*
 * Credits to Umbral Reaper and dtech in this StackOverflow post:
 * https://stackoverflow.com/questions/29196610/qt-drawing-a-filled-rounded-rectangle-with-border
*/

void DesignerIconItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option); Q_UNUSED(widget);
  painter->save();

  painter->setRenderHints(QPainter::Antialiasing);

  QPainterPath path = QPainterPath();

  QPen pen = QPen(m_borderColor, m_borderWidth);
  painter->setPen(pen);

  QBrush brush = QBrush(m_backgroundColor);
  painter->setBrush(brush);

  QRectF rect = QRectF(this->rect());
  rect.adjust(m_borderWidth / 2.0, m_borderWidth / 2.0, -m_borderWidth / 2.0, -m_borderWidth / 2.0);

  path.addRoundedRect(rect, m_borderRadius, m_borderRadius);
  painter->setClipPath(path);

  painter->fillPath(path, painter->brush());
  painter->strokePath(path, painter->pen());
  painter->drawText(rect, Qt::AlignCenter, m_text);

  painter->restore();
  AbstractDesignerItem::paint(painter, option, widget);
}

void DesignerIconItem::updateGeometry() {
  if (!m_label) return;

  const QFont font = m_label->font();
  const QFontMetrics fontMetrics(font);

  const qint32 width = fontMetrics.horizontalAdvance(m_text) + (m_padding * 2);
  const qint32 height = fontMetrics.height() + (m_padding * 2);

  m_label->resize(width, height);

  AbstractDesignerItem::updateGeometry();
}

QRectF DesignerIconItem::boundingRect() const {
  return AbstractDesignerItem::boundingRect();
}

QColor DesignerIconItem::color() const {
  return m_color;
}

void DesignerIconItem::setColor(const QColor &color) {
  m_color = color;
  update();
}

QColor DesignerIconItem::backgroundColor() const {
  return m_backgroundColor;
}

void DesignerIconItem::setBackgroundColor(const QColor &backgroundColor) {
  m_backgroundColor = backgroundColor;
  update();
}

qreal DesignerIconItem::borderRadius() const {
  return m_borderRadius;
}

void DesignerIconItem::setBorderRadius(const qreal borderRadius) {
  m_borderRadius = borderRadius;
  update();
}

QColor DesignerIconItem::borderColor() const {
  return m_borderColor;
}

void DesignerIconItem::setBorderColor(const QColor &borderColor) {
  m_borderColor = borderColor;
  update();
}

qreal DesignerIconItem::borderWidth() const {
  return m_borderWidth;
}

void DesignerIconItem::setBorderWidth(const qreal borderWidth) {
  m_borderWidth = borderWidth;
  update();
}

qreal DesignerIconItem::padding() const {
  return m_padding;
}

void DesignerIconItem::setPadding(const qreal padding) {
  m_padding = padding;
  update();
  updateGeometry();
}

REGISTER_DESIGNER_ITEM(DesignerIconItem, "BOOLEAN")
