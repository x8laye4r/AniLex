#pragma once
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QJsonObject>

#include "anilex/core/EnumConverter.h"
#include "anilex/ui/designer/DesignerType.h"

class AbstractDesignerItem : public QGraphicsProxyWidget {
  Q_OBJECT
  Q_PROPERTY(int x READ getX WRITE setX NOTIFY xChanged)
  Q_PROPERTY(int y READ getY WRITE setY NOTIFY yChanged)
  Q_PROPERTY(int width READ getWidth WRITE setWidth NOTIFY widthChanged)
  Q_PROPERTY(int height READ getHeight WRITE setHeight NOTIFY heightChanged)

signals:
  void xChanged();
  void yChanged();
  void widthChanged();
  void heightChanged();

  void propertyUpdated(int propertyIndex);

public slots:
  void resizeRect(const QRectF &resizedRect) {
    this->setPos(resizedRect.topLeft());
    this->resize(resizedRect.width(), resizedRect.height());
  }

  void finishGeometryChange() {
    const int xIdx = this->metaObject()->indexOfProperty("x");
    const int yIdx = this->metaObject()->indexOfProperty("y");
    const int widthIdx = this->metaObject()->indexOfProperty("width");
    const int heightIdx = this->metaObject()->indexOfProperty("height");

    emit propertyUpdated(xIdx);
    emit propertyUpdated(yIdx);
    emit propertyUpdated(widthIdx);
    emit propertyUpdated(heightIdx);
  }

public:                   
  explicit AbstractDesignerItem(QGraphicsItem *parent = nullptr) : QGraphicsProxyWidget(parent) {
    this->setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges | ItemSendsScenePositionChanges);
    this->setAcceptHoverEvents(false);
  }
  ~AbstractDesignerItem() override = default;

  virtual QJsonObject toJson() const {
    QJsonObject json;

    json["type"] = EnumConverter::toString(m_designerType);
    if (this->widget()) {
      qInfo() << "Setting widget pos";
      json["x"] = this->scenePos().x();
      json["y"] = this->scenePos().y();
      json["width"] = this->widget()->rect().width();
      json["height"] = this->widget()->rect().height();
    } else [[unlikely]] {
      qInfo() << "Setting general pos";
      json["x"] = 0;
      json["y"] = 0;
      json["width"] = 100;
      json["height"] = 100;
    }

    return json;
  }

  virtual void fromJson(const QJsonObject &json) {
    json.contains("type") ? m_designerType = EnumConverter::convertTo<DesignerType::DesignerItemType>(json["type"].toString()) : m_designerType = DesignerType::DesignerItemType::UNKNOWN;
    json.contains("x") ? this->setX(json["x"].toDouble()) : this->setX(0);
    json.contains("y") ? this->setY(json["y"].toDouble()) : this->setY(0);
    json.contains("width") && json.contains("height") ? this->resize(json["width"].toDouble(), json["height"].toDouble()) : this->resize(100, 100);
  }

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

  QPointF m_dragOffset;
  qreal m_zOrder;
  DesignerType::DesignerItemType m_designerType{};
  bool m_drag = false;

public:
  int getX() const { return this->scenePos().x(); }
  void setX(int x) {
    if (this->parentItem()) {
      this->parentItem()->setPos(x, this->parentItem()->y());
    } else {
      this->setPos(x, this->scenePos().y());
    }
    const int idx = this->metaObject()->indexOfProperty("x");
    // emit propertyUpdated(idx);
  }

  int getY() const { return this->scenePos().y(); }
  void setY(int y) {
    if (this->parentItem()) {
      this->parentItem()->setPos(this->scenePos().x(), y);
    } else {
      this->setPos(this->scenePos().y(), y);
    }
    const int idx = this->metaObject()->indexOfProperty("y");
    // emit propertyUpdated(idx);
  }

  int getWidth() const { return this->widget()->rect().width(); }
  void setWidth(int w) {
    if (auto *wrapper = qgraphicsitem_cast<QGraphicsRectItem*>(this->parentItem())) {
      QRectF r = wrapper->rect();
      r.setWidth(w);
      wrapper->setRect(r);
    }
    if (this->widget()) {
      this->widget()->resize(w, this->widget()->rect().height());
    }
    const int idx = this->metaObject()->indexOfProperty("width");
    // emit propertyUpdated(idx);
  }

  int getHeight() const { return this->widget()->rect().height(); }
  void setHeight(int h) {
    if (auto *wrapper = qgraphicsitem_cast<QGraphicsRectItem*>(this->parentItem())) {
      QRectF r = wrapper->rect();
      r.setHeight(h);
      wrapper->setRect(r);
    }
    if (this->widget()) {
      this->widget()->resize(this->widget()->rect().width(), h);
    }
    const int idx = this->metaObject()->indexOfProperty("height");
    // emit propertyUpdated(idx);
  }
};
