#include <QJsonObject>
#include <QLabel>

#include "anilex/ui/designer/DesignerItemFactory.h"
#include "anilex/ui/designer/item/DesignerTextItem.h"

DesignerTextItem::DesignerTextItem() {
  this->setWidget(new QLabel(tr("Some example Text")));
  m_designerType = DesignerType::DesignerItemType::TEXT;
}

void DesignerTextItem::fromJson(const QJsonObject &json) {
  const qreal x = json.contains("x") && json["x"].isDouble() ? json["x"].toDouble() : 0.0;
  const qreal y = json.contains("y") && json["y"].isDouble() ? json["y"].toDouble() : 0.0;
  this->setPos(x, y);

  const qreal width = json.contains("width") && json["width"].isDouble() ? json["width"].toDouble() : 0.0;
  const qreal height = json.contains("height") && json["height"].isDouble() ? json["height"].toDouble() : 0.0;

  if (width > 0.0 && height > 0.0) {
    this->resize(width, height);
  }

  if (json.contains("type") && json["type"].isString()) {
    m_designerType = EnumConverter::convertTo<DesignerType::DesignerItemType>(json["type"].toString());
  }

  if (json.contains("stylesheet") && json["stylesheet"].isString()) {
    QWidget* itemWidget = this->widget();
    if (itemWidget) [[likely]] {
      itemWidget->setStyleSheet(json["stylesheet"].toString());
    } else {
      qCritical("DesignerTextItem::fromJson - Internal Widget is not defined!");
    }
  }
}

QJsonObject DesignerTextItem::toJson() const {
  QJsonObject json = AbstractDesignerItem::toJson();

  if (const QWidget* itemWidget = this->widget()) [[likely]] {
    json["stylesheet"] = itemWidget->styleSheet();
  } else [[unlikely]] {
    qCritical("DesignerTextItem::toJson - Internal Widget is not defined!");
  }

  return json;
}

REGISTER_DESIGNER_ITEM(DesignerTextItem, "TEXT")
