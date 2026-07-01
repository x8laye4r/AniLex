#include "anilex/ui/designer/items/DesignerTextItem.h"
#include "anilex/ui/designer/DesignerItemFactory.h"
#include <QJsonObject>

DesignerTextItem::DesignerTextItem(QWidget *parent)
  : QLabel(parent) {
  m_type = DesignerTypes::DesignerHelpers::TEXT;
}

void DesignerTextItem::fromJson(const QJsonObject &jsonObject) {
}

QJsonObject DesignerTextItem::toJson() {
  return QJsonObject();
}

void DesignerTextItem::installEventFilters_(QObject *watched) {
  this->installEventFilter(watched);
}

QString DesignerTextItem::graphqlQuery() const {
  return m_graphqlQuery;
}

QString DesignerTextItem::setGraphqlQuery(const QString &query) {
  m_graphqlQuery = query;
}

QString DesignerTextItem::setGraphqlSource(const QString &source) {
  m_graphqlSource = source;
}

QString DesignerTextItem::graphqlSource() const {
  return m_graphqlSource;
}

bool DesignerTextItem::isSelected() const {
  return m_isSelected;
}

void DesignerTextItem::setSelected(const bool selected) {
  m_isSelected = selected;
}

void DesignerTextItem::setType(const DesignerTypes::DesignerHelpers type) {
  m_type = type;
}

DesignerTypes::DesignerHelpers DesignerTextItem::type() const {
  return m_type;
}

QColor DesignerTextItem::backgroundColor() const {
  return m_backgroundColor;
}

void DesignerTextItem::setBackgroundColor(const QColor &color) {
  if (m_backgroundColor != color) {
    m_backgroundColor = color;
    this->update();
  }
}

QColor DesignerTextItem::textColor() const {
}

void DesignerTextItem::setTextColor(const QColor &color) {
}

Qt::Alignment DesignerTextItem::textAlignment() const {
}

void DesignerTextItem::setTextAlignment(Qt::Alignment alignment) {
}

bool DesignerTextItem::bold() const {
}

void DesignerTextItem::setBold(bool bold) {
}

qint32 DesignerTextItem::fontSize() const {
}

void DesignerTextItem::setFontSize(qint32 fontSize) {
}

REGISTER_DESIGNER_ITEM(DesignerTextItem, "TEXT")
