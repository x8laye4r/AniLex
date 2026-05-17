
#include "anilex/ui/designer/DesignerWidgetsListSection.h"

#include <QJsonObject>
#include <QLabel>

#include "anilex/core/GlobalSettings.h"
#include "anilex/core/EnumConverter.h"
#include "anilex/ui/designer/DesignerWidgetListItem.h"

DesignerWidgetsListSection::DesignerWidgetsListSection(QString &name, QList<QJsonObject> &widgetsList, QWidget *parent)
  : Section(name, GlobalSettings::instance().value("Animations/duration", 100).toInt()),
            m_name(name), m_widgetsList(widgetsList) {
  m_layout = new QVBoxLayout();
  this->setCollapsableLayout();
}

void DesignerWidgetsListSection::setCollapsableLayout() {
  for (QJsonObject widgetItem : m_widgetsList) {
    QString typeStr = widgetItem.value("type").toString("TEXT");
    DesignerType::DesignerItemType type = EnumConverter::convertTo<DesignerType::DesignerItemType>(typeStr);
    
    DesignerWidgetListItem *displayName = new DesignerWidgetListItem(widgetItem.value("displayName").toString("Unknown"), type, this);
    displayName->setObjectName("designerWidgetItem");
    m_layout->addWidget(displayName);
  }
  this->setContentLayout(*m_layout);
}
