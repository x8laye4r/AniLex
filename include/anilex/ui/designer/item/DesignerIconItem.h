#pragma once
#include "anilex/ui/interfaces/AbstractDesignerItem.h"

class DesignerIconItem : public AbstractDesignerItem {
  Q_PROPERTY(bool showIcon READ showIcon WRITE setShowIcon);
  Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor);
  Q_PROPERTY(int borderThickness READ borderThickness WRITE setBorderThickness);
public:
  DesignerIconItem();
  ~DesignerIconItem() override;

  bool showIcon() const;
  void setShowIcon(bool showIcon);

  QColor borderColor() const;
  void setBorderColor(const QColor& borderColor);

  int borderThickness() const;
  void setBorderThickness(int borderThickness);

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
  QIcon m_icon;
  QColor m_borderColor = Qt::transparent;
  int m_borderThickness = 0;
  bool m_showIcon = true;
};
