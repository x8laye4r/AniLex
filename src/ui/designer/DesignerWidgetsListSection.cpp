
#include "anilex/ui/designer/DesignerWidgetsListSection.h"

#include <QJsonObject>
#include <QLabel>

#include "anilex/core/GlobalSettings.h"

DesignerWidgetsListSection::DesignerWidgetsListSection(QString &name, QList<QJsonObject> &widgetsList, QWidget *parent)
  : Section(name, GlobalSettings::instance().value("Animations/duration", 100).toInt()),
            m_name(name), m_widgetsList(widgetsList) {
  m_layout = new QVBoxLayout();
  this->setCollapsableLayout();
}

void DesignerWidgetsListSection::setCollapsableLayout() {
  for (QJsonObject widgetItem : m_widgetsList) {
    QLabel *displayName = new QLabel(widgetItem.value("displayName").toString("Unknown"), this);
    displayName->setObjectName("designerWidgetItem");
    m_layout->addWidget(displayName);
  }
  this->setContentLayout(*m_layout);
}
