#pragma once
#include <QFile>
#include <QJsonDocument>
#include <QList>
#include <QByteArray>
#include <QDebug>
#include <QJsonObject>

/**
 * @brief Parses the designer presets from a JSON file and groups them by type.
 *
 * Reads 'designer_presets.json' to extract widget definitions. The widgets are
 * categorized by their type and stored in a QMap where each key represents a
 * category and the value is a QList of associated widgets.
 * @return A QMap grouping widgets into QLists, indexed by their category type
 */
QMap<QString, QList<QJsonObject>> getDesignerWidgetsCategorized() {
  QMap<QString, QList<QJsonObject>> categorizedWidgets;

  QFile file(":/assets/designer_presets.json");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Could not open file for reading";
    return categorizedWidgets;
  }

  QByteArray rawData = file.readAll();
  file.close();

  QJsonDocument jsonDocument = QJsonDocument::fromJson(rawData);
  if (jsonDocument.isNull() || !jsonDocument.isObject()) {
    qWarning() << "Could not parse JSON document or root is not an object";
    return categorizedWidgets;
  }

  QJsonObject rootObject = jsonDocument.object();

  for (auto it = rootObject.begin(); it != rootObject.end(); ++it) {
    QString widgetName = it.key();
    QJsonObject widgetData = it.value().toObject();

    QString type = widgetData.value("type").toString("UNKNOWN");

    widgetData.insert("id", widgetName);

    categorizedWidgets[type].append(widgetData);
  }

  return categorizedWidgets;
}