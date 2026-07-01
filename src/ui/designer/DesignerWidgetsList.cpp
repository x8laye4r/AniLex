#include "anilex/ui/designer/DesignerWidgetsList.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>

#include "anilex/ui/designer/DesignerDragableLabel.h"
#include "anilex/ui/designer/DesignerWidgetListItem.h"

DesignerWidgetsList::DesignerWidgetsList(QWidget *parent)
  : QScrollArea(parent) {
  this->setWidgetResizable(true);

  m_contentWidget = new QWidget(this);
  m_contentLayout = new QVBoxLayout(m_contentWidget);
  this->setWidget(m_contentWidget);

  this->setCategories();
}



void DesignerWidgetsList::setCategories() {
  QFile file(":/designer_presets.json");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qInfo() << Q_FUNC_INFO << "Failed reading the file" << file.errorString();
    return;
  }

  QJsonParseError parseError;
  const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);

  if (parseError.error != QJsonParseError::NoError) {
    qInfo() << Q_FUNC_INFO << "Could not parse json file" << parseError.errorString();
    return;
  }

  const QJsonObject json = doc.object();

  QStringList categoryNames = json.keys();
  categoryNames.sort(Qt::CaseInsensitive);

  for (const QString &categoryName : categoryNames) {
    const QJsonObject itemsObject = json.value(categoryName).toObject();

    DesignerCategory category;
    category.categoryTitle = categoryName;

    QStringList itemKeys = itemsObject.keys();
    itemKeys.sort(Qt::CaseInsensitive);
    for (const QString &itemKey : itemKeys) {
      category.items.append(designerItemFromJsonObject(itemsObject.value(itemKey).toObject()));
    }

    auto *categoryItem = new DesignerWidgetListItem(m_contentWidget);
    categoryItem->setWidget(category);
    m_contentLayout->addWidget(categoryItem);
  }
}
