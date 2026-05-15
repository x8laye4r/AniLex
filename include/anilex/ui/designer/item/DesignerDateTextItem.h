#pragma once
#include "anilex/ui/interfaces/AbstractDesignerItem.h"


class DesignerDateTextItem : public AbstractDesignerItem {
public:
  DesignerDateTextItem();

  void fromJson(const QJsonObject &json) override;
  QJsonObject toJson() const override;
private:
  QString m_dateFormat;
};


