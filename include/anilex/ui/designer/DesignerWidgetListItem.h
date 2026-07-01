#pragma once
#include "DesignerHelpers.h"
#include "anilex/ui/Collapsable.h"
using namespace anilex_designer_helpers;

class DesignerWidgetListItem : public Section {
  Q_OBJECT
public:
  explicit DesignerWidgetListItem(QWidget *parent = nullptr);
  void setWidget(DesignerCategory category);
};
