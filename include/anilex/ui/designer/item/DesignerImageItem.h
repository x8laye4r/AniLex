#pragma once
#include "anilex/ui/CustomPixmap.h"
#include "anilex/ui/interfaces/AbstractDesignerItem.h"

class DesignerImageItem : public AbstractDesignerItem {
  Q_OBJECT

  Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius DESIGNABLE true)
  Q_PROPERTY(QColor overlayColor READ overlayColor WRITE setOverlayColor DESIGNABLE true)
  Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor DESIGNABLE true)
  Q_PROPERTY(int shadowOffsetX READ shadowOffsetX WRITE setShadowOffsetX DESIGNABLE true)
  Q_PROPERTY(int shadowOffsetY READ shadowOffsetY WRITE setShadowOffsetY DESIGNABLE true)
  Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor DESIGNABLE true)
  Q_PROPERTY(int borderThickness READ borderThickness WRITE setBorderThickness DESIGNABLE true)
public:
  DesignerImageItem();

  int cornerRadius() const;
  void setCornerRadius(const int cornerRadius);

  QColor overlayColor() const;
  void setOverlayColor(const QColor& overlayColor);

  QColor shadowColor() const;
  void setShadowColor(const QColor& shadowColor);

  int shadowOffsetX() const;
  void setShadowOffsetX(const int shadowOffsetX);

  int shadowOffsetY() const;
  void setShadowOffsetY(const int shadowOffsetY);

  QColor borderColor() const;
  void setBorderColor(const QColor& borderColor);

  int borderThickness() const;
  void setBorderThickness(const int borderThickness);

  void fromJson(const QJsonObject &json) override;
  QJsonObject toJson() const override;

private:
  CustomPixmap *m_pixmap = nullptr;
};
