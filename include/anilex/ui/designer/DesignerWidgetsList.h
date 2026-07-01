#pragma once
#include <QHBoxLayout>
#include <QScrollArea>
#include "anilex/ui/designer/DesignerHelpers.h"
using namespace anilex_designer_helpers;

class DesignerWidgetsList : public QScrollArea {
  Q_OBJECT
public:
  explicit DesignerWidgetsList(QWidget *parent = nullptr);
  ~DesignerWidgetsList() override = default;
private:
  void setCategories();

  QWidget *m_contentWidget = nullptr;
  QVBoxLayout *m_contentLayout = nullptr;
};
