#include <QJsonObject>
#include <QLabel>

#include "anilex/ui/designer/DesignerItemFactory.h"
#include "anilex/ui/designer/item/DesignerTextItem.h"

DesignerTextItem::DesignerTextItem() {
  this->setWidget(new QLabel(tr("Some example Text")));
}

void DesignerTextItem::fromJson(const QJsonObject &json) {

}

QJsonObject DesignerTextItem::toJson() const {
  QJsonObject json = AbstractDesignerItem::toJson();

  json["stylesheet"] = this->widget()->styleSheet();

  return json;
}

REGISTER_DESIGNER_ITEM(DesignerTextItem, "TEXT")
