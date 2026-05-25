#include "anilex/ui/designer/item/DesignerDateTextItem.h"

#include <QJsonObject>
#include <QDateTime>
#include <QLabel>

#include "anilex/ui/designer/DesignerItemFactory.h"

DesignerDateTextItem::DesignerDateTextItem() {
  m_dateFormat = "yyyy-MM-dd";
  QDateTime time = QDateTime::currentDateTime();
  QLabel *label = new QLabel(time.toString(m_dateFormat));
  this->setWidget(label);
}

QString DesignerDateTextItem::dateFormat() const {
  return m_dateFormat;
}

void DesignerDateTextItem::setDateFormat(const QString &dateFormat) {
  const QDateTime time = QDateTime::currentDateTime();

  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    const QString formattedDate = time.toString(dateFormat);

    if (dateFormat.isEmpty() || formattedDate.isEmpty()) {
      label->setText(time.toString("yyyy-MM-dd"));
      m_dateFormat = "yyyy-MM-dd";
    } else {
      label->setText(formattedDate);
      m_dateFormat = formattedDate;
    }
  }
}

QColor DesignerDateTextItem::textColor() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QPalette palette = label->palette();
    return palette.color(QPalette::WindowText);
  }
  qCritical("DesignerDateTextItem::textColor - Internal Widget is not defined!");
  return QColor();
}

void DesignerDateTextItem::setTextColor(const QColor &textColor) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QPalette palette = label->palette();
    palette.setColor(QPalette::WindowText, textColor);
    label->setPalette(palette);
  } else {
    qCritical("DesignerDateTextItem::setTextColor - Internal Widget is not defined!");
  }
}

QColor DesignerDateTextItem::backgroundColor() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QPalette palette = label->palette();
    return palette.color(QPalette::Window);
  }
  qCritical("DesignerDateTextItem::backgroundColor - Internal Widget is not defined!");
  return QColor();
}

void DesignerDateTextItem::setBackgroundColor(const QColor &backgroundColor) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QPalette palette = label->palette();
    palette.setColor(QPalette::Window, backgroundColor);
    label->setPalette(palette);
  } else {
    qCritical("DesignerDateTextItem::setBackgroundColor - Internal Widget is not defined!");
  }
}

QString DesignerDateTextItem::stylesheet() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    return this->widget()->styleSheet();
  }
  qCritical("DesignerDateTextItem::stylesheet - Internal Widget is not defined!");
  return QString();
}

void DesignerDateTextItem::setStylesheet(const QString &stylesheet) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    label->setStyleSheet(stylesheet);
  } else {
    qCritical("DesignerDateTextItem::setStylesheet - Internal Widget is not defined!");
  }
}

Qt::Alignment DesignerDateTextItem::alignment() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    return label->alignment();
  }
  qCritical("DesignerDateTextItem::alignment - Internal Widget is not defined!");
  return Qt::AlignCenter;
}

void DesignerDateTextItem::setAlignment(const Qt::Alignment &textAlignment) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    label->setAlignment(textAlignment);
  } else {
    qCritical("DesignerDateTextItem::setAlignment - Internal Widget is not defined!");
  }
}

bool DesignerDateTextItem::bold() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QFont font = label->font();
    return font.bold();
  }
  qCritical("DesignerDateTextItem::bold - Internal Widget is not defined!");
  return false;
}

void DesignerDateTextItem::setBold(bool bold) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QFont font = label->font();
    font.setBold(bold);
    label->setFont(font);
  } else {
    qCritical("DesignerDateTextItem::setBold - Internal Widget is not defined!");
  }
}

int DesignerDateTextItem::margin() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    return label->margin();
  }
  qCritical("DesignerDateTextItem::margin - Internal Widget is not defined!");
  return 5;
}

void DesignerDateTextItem::setMargin(int margin) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    label->setMargin(margin);
  } else {
    qCritical("DesignerDateTextItem::setMargin - Internal Widget is not defined!");
  }
}

int DesignerDateTextItem::fontSize() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QFont font = label->font();
    return font.pointSize();
  }
  qCritical("DesignerDateTextItem::font - Internal Widget is not defined!");
  return 12;
}

void DesignerDateTextItem::setFontSize(int fontSize) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QFont font = label->font();
    font.setPointSize(fontSize);
    label->setFont(font);
  } else {
    qCritical("DesignerDateTextItem::setFontSize - Internal Widget is not defined!");
  }
}

int DesignerDateTextItem::zValue() const {
  return AbstractDesignerItem::zValue();
}

void DesignerDateTextItem::setZValue(int zValue) {
  AbstractDesignerItem::setZValue(zValue);
}

void DesignerDateTextItem::fromJson(const QJsonObject &json) {
}

QJsonObject DesignerDateTextItem::toJson() const {
  QJsonObject json = AbstractDesignerItem::toJson();

  const QMetaObject *metaObject = this->metaObject();
  const int count = metaObject->propertyCount();
  const int countOffset = metaObject->propertyOffset();

  for (int i = countOffset; i < count; i++) {
    QMetaProperty property = metaObject->property(i);
    if (property.isValid() && property.isReadable()) {
      json[property.name()] = QJsonValue::fromVariant(property.read(this));
    }
  }
  return json;
}

REGISTER_DESIGNER_ITEM(DesignerDateTextItem, "DATE_TEXT")
