#pragma once
#include <QFrame>
#include <QLabel>
#include <QList>
#include <QJsonObject>

#include "DesignerWidgetsList.h"

class DesignerView;

class Designer : public QFrame {
  Q_OBJECT
public:
  explicit Designer(QWidget* parent = nullptr);
private:
  DesignerView *m_designerView;
  DesignerWidgetsList *m_widgetList;
  QLabel *m_widgetListLabel;

  QHBoxLayout *m_frameLayout;
  QVBoxLayout *m_viewLayout;

  void setupUi();

public slots:
  void exportWidgetsAsJson();
  void openDesigner();
  void closeDesigner();
};
