#include "anilex/ui/designer/item/DesignerScrollableItem.h"
#include <QScrollArea>

DesignerScrollableItem::DesignerScrollableItem() {
  QScrollArea *scrollArea = new QScrollArea();
  scrollArea->setWidgetResizable(true);
  scrollArea->resize(80, 40);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  QWidget *contentWidget = new QWidget();


  for (int i = 0; i < 5; i++) {

  }

  this->setWidget(scrollArea);
}

Qt::Orientation DesignerScrollableItem::orientation() const {
  if (QScrollArea *scrollArea = qobject_cast<QScrollArea *>(this->widget())) {
    if (scrollArea->horizontalScrollBar() == Qt::ScrollBarAsNeeded) {
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
  if (QScrollArea *scrollArea = qobject_cast<QScrollArea *>(this->widget())) {
     if (orientation == Qt::Horizontal) {
      scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
      scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    } else {
      scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
  } else {
    qCritical() << "DesignerScrollableItem::setOrientation - Core Widget not set!";
  }
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
  if (QScrollArea *scrollArea = qobject_cast<QScrollArea *>(this->widget())) {

  }
  qCritical() << "DesignerScrollableItem::showScrollBar - Core Widget not set!";
  return true;
}

void DesignerScrollableItem::setShowScrollBar(bool show) {
}

QString DesignerScrollableItem::stylesheet() const {
}

void DesignerScrollableItem::setStylesheet(QString stylesheet) {
}

QColor DesignerScrollableItem::textColor() const {
}

void DesignerScrollableItem::setTextColor(QColor color) {
}

QColor DesignerScrollableItem::backgroundColor() const {
}

void DesignerScrollableItem::setBackgroundColor(QColor color) {
}

QString DesignerScrollableItem::itemStylesheet() const {
}

void DesignerScrollableItem::setItemStylesheet(QString stylesheet) {
}

Qt::Alignment DesignerScrollableItem::alignment() const {
}

void DesignerScrollableItem::setAlignment(Qt::Alignment alignment) {
}

int DesignerScrollableItem::margin() const {
}

void DesignerScrollableItem::setMargin(int margin) {
}

int DesignerScrollableItem::fontSize() const {
}

void DesignerScrollableItem::setFontSize(int size) {
}

void DesignerScrollableItem::fromJson(const QJsonObject &json) {
}

QJsonObject DesignerScrollableItem::toJson() const {
  return AbstractDesignerItem::toJson();
}
