#include "anilex/ui/designer/DesignerWidgetsList.h"

#include <QJsonObject>

#include "anilex/ui/designer/DesignerWidgetsListSection.h"

DesignerWidgetsList::DesignerWidgetsList(QWidget *parent)
  : QScrollArea(parent) {
  m_centralWidget = new QWidget(this);
  m_centralWidgetLayout = new QVBoxLayout(m_centralWidget);

  m_centralWidgetLayout->setAlignment(Qt::AlignTop);

  m_centralWidget->setLayout(m_centralWidgetLayout);

  this->setWidget(m_centralWidget);
  this->setWidgetResizable(true);
}

void DesignerWidgetsList::addWidgetSections(QMap<QString, QList<QJsonObject>> &widgetsList) {
  for (auto item = widgetsList.begin(); item != widgetsList.end(); ++item) {
    QString name = item.key();
    QList<QJsonObject> obj = item.value();

    DesignerWidgetsListSection *section = new DesignerWidgetsListSection(name, obj, this);

    m_centralWidgetLayout->addWidget(section);
  }
}
