#pragma once
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QJsonObject>

#include "anilex/core/EnumConverter.h"
#include "anilex/ui/designer/DesignerType.h"

class AbstractDesignerItem : public QGraphicsProxyWidget {
  Q_OBJECT
public:                   
  explicit AbstractDesignerItem(QGraphicsItem *parent = nullptr) : QGraphicsProxyWidget(parent) {
    this->setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges | ItemSendsScenePositionChanges);
  }

  virtual QJsonObject toJson() const {
    QJsonObject json;

    json["type"] = EnumConverter::toString(m_designerType);
    json["x"] = this->pos().x();
    json["y"] = this->pos().y();

    json["width"] = this->rect().width();
    json["height"] = this->rect().height();

    return json;
  }

  virtual void fromJson(const QJsonObject &json) = 0;

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
    QGraphicsProxyWidget::paint(painter, option, widget);

    if (this->isSelected()) {
      painter->save();

      QRectF boundingRect = this->boundingRect();

      // draw outline
      painter->setPen(QPen(Qt::white, 2, Qt::SolidLine));
      painter->setBrush(Qt::NoBrush);
      painter->drawRect(boundingRect);

      // draw corner rects to resize
      const qreal xLeft = boundingRect.left() - 5;
      const qreal xRight = boundingRect.right() - 5;
      const qreal yTop = boundingRect.top() - 5;
      const qreal yBottom = boundingRect.bottom() - 5;

      const QRectF rects[] = {
        QRectF(xLeft, yTop,    10, 10), // bottom left
        QRectF(xLeft,yBottom, 10, 10), // top left
        QRectF(xRight,yTop,    10, 10), // top right
        QRectF(xRight, yBottom, 10, 10)  // bottom right
    };

      painter->setBrush(Qt::white);
      painter->drawRects(rects, std::size(rects));

      painter->restore();
    }
  }

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      m_drag = true;
      m_dragOffset = event->pos();
      event->accept();
      return;
    }
    QGraphicsProxyWidget::mousePressEvent(event);
  }

  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override {
    if (m_drag && event->button() == Qt::LeftButton) {
      QPointF newPos = event->scenePos() - m_dragOffset;
      setPos(newPos);
      event->accept();
      return;
    }
    QGraphicsProxyWidget::mouseMoveEvent(event);
  }

  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      m_drag = false;
      event->accept();
      return;
    }
    QGraphicsProxyWidget::mouseReleaseEvent(event);
  }

protected:
  QPointF m_dragOffset;
  qreal m_zOrder;
  DesignerType::DesignerItemType m_designerType{};
  bool m_drag = false;
};
