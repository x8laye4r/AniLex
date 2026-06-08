#pragma once
#include "DesignerWidgetsList.h"
#include "anilex/ui/Collapsable.h"

class DesignerWidgetsListSection : public Section {
  Q_OBJECT
public:
  explicit DesignerWidgetsListSection(const QString &name, const QList<QJsonObject> &widgetsList, QWidget *parent = nullptr);

private:
  QString m_name;
  QList<QJsonObject> m_widgetsList;
  QVBoxLayout* m_layout = nullptr;

  void setCollapsableLayout();

};
