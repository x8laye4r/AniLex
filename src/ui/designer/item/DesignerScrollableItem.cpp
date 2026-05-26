#include "anilex/ui/designer/item/DesignerScrollableItem.h"

#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>

#include "anilex/ui/designer/DesignerItemFactory.h"

DesignerScrollableItem::DesignerScrollableItem() {
  QScrollArea *scrollArea = new QScrollArea();

  scrollArea->setWidgetResizable(true);
  scrollArea->resize(80, 40);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  QWidget *contentWidget = new QWidget();
  QHBoxLayout *layout = new QHBoxLayout(contentWidget);
  scrollArea->setWidget(contentWidget);

  for (int i = 0; i < 5; ++i) {
    QLabel *exampleLabel = new QLabel("Test Item");
    contentWidget->layout()->addWidget(exampleLabel);
  }
  updateItemsLayout();
  updateItemStyle();
  this->setWidget(scrollArea);
  this->resize(85, 45);
}

Qt::Orientation DesignerScrollableItem::orientation() const {
  if (QScrollArea *scrollArea = qobject_cast<QScrollArea *>(this->widget())) {
    if (HORIZONTAL_SCROLLBAR_ON(scrollArea)) {
      return Qt::Horizontal;
    }
    return Qt::Vertical;
  }
  qCritical() << "DesignerScrollableItem::orientation - Core Widget not set!";
  return Qt::Horizontal;
}

void DesignerScrollableItem::setOrientation(Qt::Orientation orientation) {
  if (this->orientation() == orientation) {
    return;
  }
  QScrollArea *scrollArea = qobject_cast<QScrollArea *>(this->widget());
  if (!scrollArea || scrollArea->widget()) {
    qCritical() << "DesignerScrollableItem::setOrientation - Core Widget not set!";
    return;
  }

  QWidget *contentWidget = scrollArea->widget();
  QLayout *oldLayout = contentWidget->layout();
  QList<QWidget *> items;

  if (oldLayout) {
    QLayoutItem *item;
    while ((item = oldLayout->takeAt(0))) {
      if (QWidget *w = item->widget()) {
        items.append(w);
      }
      delete item;
    }
    delete oldLayout;
  }

  if (orientation == Qt::Horizontal) {
    if (m_showScrollBar) {
      scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QHBoxLayout *newLayout = new QHBoxLayout(contentWidget);
    for (QWidget *widget : items) {
      newLayout->addWidget(widget);
    }
  } else {
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if (m_showScrollBar) {
      scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    QVBoxLayout *newLayout = new QVBoxLayout(contentWidget);
    for (QWidget *widget : items) {
      newLayout->addWidget(widget);
    }
  }
  updateItemsLayout();
}

bool DesignerScrollableItem::widgetResizable() const {
  if (QScrollArea *scrollArea = qobject_cast<QScrollArea *>(this->widget())) {
    return scrollArea->widgetResizable();
  }
  qCritical() << "DesignerScrollableItem::widgetResizable - Core Widget not set!";
  return true;
}

void DesignerScrollableItem::setWidgetResizable(bool resizable) {
  if (QScrollArea *scrollArea = qobject_cast<QScrollArea *>(this->widget())) {
    scrollArea->setWidgetResizable(resizable);
  } else {
    qCritical() << "DesignerScrollableItem::setWidgetResizable - Core Widget not set!";
  }
}

bool DesignerScrollableItem::showScrollBar() const {
  return m_showScrollBar;
}

void DesignerScrollableItem::setShowScrollBar(bool show) {
  m_showScrollBar = show;
  if (QScrollArea *scrollArea = qobject_cast<QScrollArea *>(this->widget())) {
    if (orientation() == Qt::Horizontal) {
      scrollArea->setHorizontalScrollBarPolicy(show ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
    } else {
      scrollArea->setVerticalScrollBarPolicy(show ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
    }
  } else {
    qCritical() << "DesignerScrollableItem::setShowScrollBar - Core Widget not set!";
  }
}

QString DesignerScrollableItem::stylesheet() const {
  if (QWidget *widget = this->widget()) {
    return widget->styleSheet();
  }
  qCritical() << "DesignerScrollableItem::stylesheet - Core Widget not set!";
  return QString();
}

void DesignerScrollableItem::setStylesheet(const QString &stylesheet) {
  if (QWidget *widget = this->widget()) {
    widget->setStyleSheet(stylesheet);
  } else {
    qCritical() << "DesignerScrollableItem::setStylesheet - Core Widget not set!";
  }
}

QColor DesignerScrollableItem::textColor() const {
  return m_textColor;
}

void DesignerScrollableItem::setTextColor(QColor color) {
  m_textColor = color;
  updateItemStyle();
}

QColor DesignerScrollableItem::backgroundColor() const {
  return m_textBackgroundColor;
}

void DesignerScrollableItem::setBackgroundColor(QColor color) {
  m_textBackgroundColor = color;
  updateItemStyle();
}

QString DesignerScrollableItem::itemStylesheet() const {
  return m_itemStylesheet;
}

void DesignerScrollableItem::setItemStylesheet(const QString &stylesheet) {
  m_itemStylesheet = stylesheet;
  updateItemStyle();
}

Qt::Alignment DesignerScrollableItem::alignment() const {
  return m_alignment;
}

void DesignerScrollableItem::setAlignment(Qt::Alignment alignment) {
  m_alignment = alignment;
  updateItemStyle();
}

int DesignerScrollableItem::margin() const {
  return m_margin;
}

void DesignerScrollableItem::setMargin(int margin) {
  m_margin = margin;
  updateItemStyle();
}

int DesignerScrollableItem::fontSize() const {
  return m_fontSize;
}

void DesignerScrollableItem::setFontSize(int size) {
  m_fontSize = size;
  updateItemStyle();
}

void DesignerScrollableItem::fromJson(const QJsonObject &json) {
  AbstractDesignerItem::fromJson(json);

  for (auto it = json.constBegin(); it != json.constEnd(); ++it) {
    const QString name = it.key();
    const QVariant value = it.value().toVariant();
    this->setProperty(name.toUtf8().constData(), value);
  }
}

QJsonObject DesignerScrollableItem::toJson() const {
  QJsonObject json = AbstractDesignerItem::toJson();

  const QMetaObject *metaObject = this->metaObject();
  const int count = metaObject->propertyCount();
  const int offset = metaObject->propertyOffset();

  for (int i = offset; i < count; i++) {
    const QMetaProperty property = metaObject->property(i);
    if (property.isValid() && property.isReadable()) {
      json[property.name()] = QJsonValue::fromVariant(property.read(this));
    }
  }

  return json;
}

void DesignerScrollableItem::updateItemsLayout() {
  QScrollArea *scrollArea = qobject_cast<QScrollArea *>(this->widget());
  if (!scrollArea || !scrollArea->widget()) return;

  QLayout *layout = scrollArea->widget()->layout();
  layout->setContentsMargins(m_margin, m_margin, m_margin, m_margin);
  layout->setAlignment(m_alignment);
}

void DesignerScrollableItem::updateItemStyle() {
  QScrollArea *scrollArea = qobject_cast<QScrollArea *>(this->widget());
  if (!scrollArea || !scrollArea->widget()) return;

  QString style = m_itemStylesheet;
  if (style.isEmpty()) {
    style = QString("QWidget { background-color: %1; color: %2; font-size: %3px; border: 1px solid gray; }")
            .arg(m_textBackgroundColor.name())
            .arg(m_textColor.name())
            .arg(m_fontSize);
  }

  const auto children = scrollArea->widget()->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly);
  for (QWidget *child : children) {
    child->setStyleSheet(style);
  }
}

REGISTER_DESIGNER_ITEM(DesignerScrollableItem, "SCROLLABLE")
