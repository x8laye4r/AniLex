#pragma once
#include <QGraphicsProxyWidget>
#include <QPainter>

#include "anilex/ui/designer/DesignerType.h"

class AbstractDesignerItem : public QGraphicsProxyWidget {
  Q_OBJECT
public:
  explicit AbstractDesignerItem(QGraphicsItem* parent = nullptr) : QGraphicsProxyWidget(parent) {
    this->setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges | ItemSendsScenePositionChanges);
  }

  virtual QJsonObject toJson() = 0;
  virtual void fromJson(const QJsonObject &json) = 0;

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
    QGraphicsProxyWidget::paint(painter, option, widget);

    if (this->isSelected()) {
      painter->save();
      painter->setPen(QPen(Qt::white, 2, Qt::SolidLine));
      painter->setBrush(Qt::NoBrush);
      painter->drawRect(this->boundingRect());
      painter->restore();
    }
  }

private:
  qreal m_zOrder;
  DesignerType::DesignerItemType m_designerType;
};
