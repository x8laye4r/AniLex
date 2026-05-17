#pragma once
#include <QLabel>
#include <QJsonObject>
#include <QPushButton>

#include "anilex/ui/designer/DesignerWidgetsList.h"
#include "anilex/ui/designer/DesignerView.h"
#include "anilex/core/AniListEnums.h"
#include "anilex/ui/designer/DesignerType.h"

class Designer : public QFrame {
  Q_OBJECT
public:
  explicit Designer(QWidget* parent = nullptr);
private:
  DesignerView *m_designerView;
  DesignerWidgetsList *m_widgetList;
  QLabel *m_widgetListLabel;
  QPushButton *m_exportCardButton;

  QHBoxLayout *m_frameLayout;
  QVBoxLayout *m_viewLayout;
  QVBoxLayout *m_listViewLayout;
  QVBoxLayout *m_propertiesLayout;

  void setupUi() const;
  void setupObjectNames();
  void setupConnections();

public slots:
  void exportWidgetsAsJson(DesignerType::DesignerCreatorItems cardToExportFor) const;
  void openDesigner();
  void closeDesigner();
  void clearView();
};
