#pragma once
#include <QFrame>
#include <QLabel>
#include <QList>
#include <QJsonObject>

#include "DesignerWidgetsList.h"
#include "anilex/core/AniListEnums.h"

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
  void exportWidgetsAsJson(AniListEnums::MediaStatus cardToExportFor) const;
  void openDesigner();
  void closeDesigner();
  void clearView();
};
