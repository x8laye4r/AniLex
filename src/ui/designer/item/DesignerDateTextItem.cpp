#include "anilex/ui/designer/item/DesignerDateTextItem.h"

#include <QJsonObject>

DesignerDateTextItem::DesignerDateTextItem() {
}

void DesignerDateTextItem::fromJson(const QJsonObject &json) {
}

QJsonObject DesignerDateTextItem::toJson() const {
  return AbstractDesignerItem::toJson();
}
