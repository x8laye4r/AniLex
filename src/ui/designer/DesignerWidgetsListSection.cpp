#include "anilex/ui/designer/DesignerWidgetsListSection.h"

#include <QCoreApplication>
#include <QJsonObject>

#include "anilex/core/GlobalSettings.h"
#include "anilex/core/EnumConverter.h"
#include "anilex/ui/designer/DesignerWidgetListItem.h"

DesignerWidgetsListSection::DesignerWidgetsListSection(const QString &name, const QList<QJsonObject> &widgetsList, QWidget *parent)
  : Section(QCoreApplication::translate("DesignerPresetsTitles", name.toUtf8().constData()), GlobalSettings::instance().value("Animations/duration", 100).toInt()),
            m_name(name), m_widgetsList(widgetsList) {
  m_layout = new QVBoxLayout();
  this->setCollapsableLayout();
}

void DesignerWidgetsListSection::setCollapsableLayout() {
  for (const QJsonObject& widgetItem : m_widgetsList) {
    QString typeStr = widgetItem.value("type").toString("TEXT");
    const DesignerType::DesignerItemType type = EnumConverter::convertTo<DesignerType::DesignerItemType>(typeStr);
    
    QString rawName = widgetItem.value("displayName").toString("Unknown");
    QString translatedName = QCoreApplication::translate("DesignerPresets", rawName.toUtf8().constData());

    DesignerWidgetListItem *displayName = new DesignerWidgetListItem(translatedName, type, this);
    displayName->setObjectName("designerWidgetItem");
    m_layout->addWidget(displayName);
  }
  this->setContentLayout(*m_layout);
}

void dummy_json_translations2() {
  QT_TRANSLATE_NOOP("DesignerPresetsTitles", "TEXT");
  QT_TRANSLATE_NOOP("DesignerPresetsTitles", "DATE_TEXT");
  QT_TRANSLATE_NOOP("DesignerPresetsTitles", "SCROLLABLE");
  QT_TRANSLATE_NOOP("DesignerPresetsTitles", "BOOLEAN");
  QT_TRANSLATE_NOOP("DesignerPresetsTitles", "IMAGE");
}
