#pragma once
#include "anilex/ui/Collapsable.h"

class DesignerWidgetListItem : public Section {
  Q_OBJECT
public:
  explicit DesignerWidgetListItem(const QString &title, QWidget *parent = nullptr);
  void setWidgets(QStringList &widgetNames);
};
