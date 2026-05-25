#include <QJsonObject>
#include <QLabel>

#include "anilex/ui/designer/DesignerItemFactory.h"
#include "anilex/ui/designer/item/DesignerTextItem.h"

DesignerTextItem::DesignerTextItem() {
  QLabel *temp = new QLabel(tr("Some example Text"));
  temp->setWordWrap(true);
  this->setWidget(temp);
  m_designerType = DesignerType::DesignerItemType::TEXT;
}

QString DesignerTextItem::text() const {
  QLabel *label = qobject_cast<QLabel *>(this->widget());
  return label ? label->text() : QString();
}

void DesignerTextItem::setText(const QString &text) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    label->setText(text);
  } else {
    qCritical() << "DesignerTextItem::setText - Internal Widget is not defined!";
  }
}

QColor DesignerTextItem::textColor() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QPalette palette = label->palette();
    return palette.color(QPalette::WindowText);
  }
  qCritical() << "DesignerTextItem::textColor - Internal Widget is not defined!";
  return QColor();
}

void DesignerTextItem::setTextColor(const QColor &textColor) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QPalette palette = label->palette();
    palette.setColor(QPalette::WindowText, textColor);
    label->setPalette(palette);
  } else {
    qCritical() << "DesignerTextItem::setTextColor - Internal Widget is not defined!";
  }
}

QColor DesignerTextItem::backgroundColor() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QPalette palette = label->palette();
    return palette.color(QPalette::Window);
  }
  qCritical() << "DesignerTextItem::backgroundColor - Internal Widget is not defined!";
  return QColor();
}

void DesignerTextItem::setBackgroundColor(const QColor &backgroundColor) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QPalette palette = label->palette();
    palette.setColor(QPalette::Window, backgroundColor);
    label->setPalette(palette);
    label->setAutoFillBackground(true);
  } else {
    qCritical() << "DesignerTextItem::setBackgroundColor - Internal Widget is not defined!";
  }
}

QString DesignerTextItem::stylesheet() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    return label->styleSheet();
  }
  qCritical() << "DesignerTextItem::stylesheet - Internal Widget is not defined!";
  return QString();
}

void DesignerTextItem::setStylesheet(const QString &stylesheet) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    label->setStyleSheet(stylesheet);
  } else {
    qCritical() << "DesignerTextItem::setStylesheet - Internal Widget is not defined!";
  }
}

Qt::Alignment DesignerTextItem::alignment() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    return label->alignment();
  }
  qCritical() << "DesignerTextItem::alignment - Internal Widget is not defined!";
  return Qt::AlignCenter;
}

void DesignerTextItem::setAlignment(Qt::Alignment alignment) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    label->setAlignment(alignment);
  } else {
    qCritical() << "DesignerTextItem::setAlignment() - Internal Widget is not defined!";
  }
}

bool DesignerTextItem::bold() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QFont font = label->font();
    return font.bold();
  }
  qCritical() << "DesignerTextItem::bold() - Internal Widget is not defined!";
  return false;
}

void DesignerTextItem::setBold(bool bold) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QFont font = label->font();
    font.setBold(bold);
    label->setFont(font);
  } else {
    qCritical() << "DesignerTextItem::setBold() - Internal Widget is not defined!";
  }
}

bool DesignerTextItem::wordWrap() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    return label->wordWrap();
  }
  qCritical() << "DesignerTextItem::wordWrap() - Internal Widget is not defined!";
  return false;
}

void DesignerTextItem::setWordWrap(bool wordWrap) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    label->setWordWrap(wordWrap);
  } else {
    qCritical() << "DesignerTextItem::setWordWrap() - Internal Widget is not defined!";
  }
}

int DesignerTextItem::margin() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    return label->margin();
  }
  qCritical() << "DesignerTextItem::margin() - Internal Widget is not defined!";
  return 5;
}

void DesignerTextItem::setMargin(int margin) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    label->setMargin(margin);
  } else {
    qCritical() << "DesignerTextItem::setMargin() - Internal Widget is not defined!";
  }
}

int DesignerTextItem::fontSize() const {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QFont font = label->font();
    return font.pointSize();
  }
  qCritical() << "DesignerTextItem::fontSize() - Internal Widget is not defined!";
  return 12;
}

void DesignerTextItem::setFontSize(int fontSize) {
  if (QLabel *label = qobject_cast<QLabel *>(this->widget())) {
    QFont font = label->font();
    font.setPointSize(fontSize);
    label->setFont(font);
  } else {
    qCritical() << "DesignerTextItem::setFontSize() - Internal Widget is not defined!";
  }
}

int DesignerTextItem::zValue() const {
  return QGraphicsProxyWidget::zValue();
}

void DesignerTextItem::setZValue(const int zValue) {
  QGraphicsProxyWidget::setZValue(zValue);
}

void DesignerTextItem::fromJson(const QJsonObject &json) {
  const qreal x = json.contains("x") && json["x"].isDouble() ? json["x"].toDouble() : 0.0;
  const qreal y = json.contains("y") && json["y"].isDouble() ? json["y"].toDouble() : 0.0;
  this->setPos(x, y);

  const qreal width = json.contains("width") && json["width"].isDouble() ? json["width"].toDouble() : 0.0;
  const qreal height = json.contains("height") && json["height"].isDouble() ? json["height"].toDouble() : 0.0;

  if (width > 0.0 && height > 0.0) {
    this->resize(width, height);
  }

  if (json.contains("type") && json["type"].isString()) {
    m_designerType = EnumConverter::convertTo<DesignerType::DesignerItemType>(json["type"].toString());
  }

  if (json.contains("stylesheet") && json["stylesheet"].isString()) {
    QWidget* itemWidget = this->widget();
    if (itemWidget) [[likely]] {
      itemWidget->setStyleSheet(json["stylesheet"].toString());
    } else {
      qCritical("DesignerTextItem::fromJson - Internal Widget is not defined!");
    }
  }
}

QJsonObject DesignerTextItem::toJson() const {
  QJsonObject json = AbstractDesignerItem::toJson();

  const QMetaObject *metaObject = this->metaObject();
  const int propertyCount = metaObject->propertyCount();
  const int propertyOffset = metaObject->propertyOffset();

  for (int i = propertyOffset; i < propertyCount; i++) {
    QMetaProperty property = metaObject->property(i);
    if (property.isValid() && property.isReadable()) {
      QVariant value = property.read(this);
      json[property.name()] = QJsonValue::fromVariant(value);
    }
  }

  return json;
}

REGISTER_DESIGNER_ITEM(DesignerTextItem, "TEXT")
