#pragma once
#include "DesignerWidgetsList.h"
#include "anilex/ui/Collapsable.h"

class DesignerWidgetsListSection : public Section {
public:
  explicit DesignerWidgetsListSection(QString &name, QList<QJsonObject> &widgetsList, QWidget *parent = nullptr);

private:
  QString m_name;
  QList<QJsonObject> m_widgetsList;
  QVBoxLayout* m_layout = nullptr;

  void setCollapsableLayout();

};
