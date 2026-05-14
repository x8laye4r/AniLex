#pragma once
#include <QFrame>

#include "DesignerWidgetsList.h"

class DesignerView;

class Designer : public QFrame {
  Q_OBJECT
public:
  explicit Designer(QWidget* parent = nullptr);
private:
  DesignerView *m_designerView;
  DesignerWidgetsList *m_widgetList;

  void setupUi();
signals:
  // void openDesigner();
  // void closeDesigner();
};
