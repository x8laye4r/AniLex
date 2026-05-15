#pragma once
#include "anilex/ui/interfaces/AbstractDesignerItem.h"


class DesignerTextItem : public AbstractDesignerItem {
  Q_OBJECT
public:
  DesignerTextItem();

  void fromJson(const QJsonObject &json) override;
  QJsonObject toJson() const override;
};
