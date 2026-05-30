#include "anilex/ui/designer/item/DesignerIconItem.h"
#include "anilex/ui/designer/DesignerItemFactory.h"


DesignerIconItem::DesignerIconItem() {
  m_designerType = DesignerType::DesignerItemType::BOOLEAN;
}

DesignerIconItem::~DesignerIconItem() = default;

bool DesignerIconItem::showIcon() const {
  return m_showIcon;
}

void DesignerIconItem::setShowIcon(bool showIcon) {
  m_showIcon = showIcon;
}

QColor DesignerIconItem::borderColor() const {
  return m_borderColor;
}

void DesignerIconItem::setBorderColor(const QColor &borderColor) {
  m_borderColor = borderColor;
}

int DesignerIconItem::borderThickness() const {
  return m_borderThickness;
}

void DesignerIconItem::setBorderThickness(int borderThickness) {
  m_borderThickness = borderThickness;
}

void DesignerIconItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option); Q_UNUSED(widget);

  painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

  QRectF contentRect = this->rect();

  if (m_borderColor != Qt::transparent && m_borderThickness > 0) {
    painter->save();
    QPen pen(m_borderColor, m_borderThickness);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawPath(shape());
    painter->restore();

    qreal halfThickness = m_borderThickness / 2.0;
    contentRect.adjust(halfThickness, halfThickness, -halfThickness, -halfThickness);
  }

  painter->save();

  if (!m_icon.isNull()) {
    painter->drawImage(contentRect, m_icon.pixmap(contentRect.size().toSize()).toImage());
  } else {
    painter->fillRect(contentRect, QColor(240, 240, 240));
  }
  painter->restore();
}

REGISTER_DESIGNER_ITEM(DesignerIconItem, "ICON")
