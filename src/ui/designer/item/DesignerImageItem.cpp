#include "anilex/ui/designer/item/DesignerImageItem.h"
#include "anilex/ui/designer/DesignerItemFactory.h"

DesignerImageItem::DesignerImageItem() {
  m_pixmap = new CustomPixmap();

  m_designerType = DesignerType::DesignerItemType::IMAGE;

  this->setWidget(m_pixmap);
}

int DesignerImageItem::cornerRadius() const {
  return m_pixmap->cornerRadius();
}

void DesignerImageItem::setCornerRadius(const int cornerRadius) {
  m_pixmap->setRoundedCorners(cornerRadius);
}

QColor DesignerImageItem::overlayColor() const {
  return m_pixmap->overlayColor();
}

void DesignerImageItem::setOverlayColor(const QColor &overlayColor) {
  m_pixmap->setOverlay(overlayColor);
}

QColor DesignerImageItem::shadowColor() const {
  return m_pixmap->shadowColor();
}

void DesignerImageItem::setShadowColor(const QColor &shadowColor) {
  m_pixmap->setShadow(shadowColor);
}

int DesignerImageItem::shadowOffsetX() const {
  return m_pixmap->shadowOffsetX();
}

void DesignerImageItem::setShadowOffsetX(const int shadowOffsetX) {
  m_pixmap->setOffsetShadow(shadowOffsetX, m_pixmap->shadowOffsetY());
}

int DesignerImageItem::shadowOffsetY() const {
  return m_pixmap->shadowOffsetY();
}

void DesignerImageItem::setShadowOffsetY(const int shadowOffsetY) {
  m_pixmap->setOffsetShadow(m_pixmap->shadowOffsetX(), shadowOffsetY);
}

QColor DesignerImageItem::borderColor() const {
  return m_pixmap->borderColor();
}

void DesignerImageItem::setBorderColor(const QColor &borderColor) {
  m_pixmap->setBorder(borderColor);
}

int DesignerImageItem::borderThickness() const {
  return m_pixmap->borderThickness();
}

void DesignerImageItem::setBorderThickness(const int borderThickness) {
  m_pixmap->setBorderThickness(borderThickness);
}

void DesignerImageItem::fromJson(const QJsonObject &json) {
  AbstractDesignerItem::fromJson(json);
}

QJsonObject DesignerImageItem::toJson() const {
  QJsonObject json = AbstractDesignerItem::toJson();

  const QMetaObject *metaObject = this->metaObject();
  const int count = metaObject->propertyCount();
  const int offset = metaObject->propertyOffset();

  for (int i = offset; i < count; ++i) {
    QMetaProperty property = metaObject->property(i);
    if (property.isValid() && property.isReadable()) {
      json[property.name()] = QJsonValue::fromVariant(property.read(this));
    }
  }

  return json;
}

REGISTER_DESIGNER_ITEM(DesignerImageItem, "IMAGE")
