#pragma once
#include <QWidget>
#include <QRubberBand>

#include "IDesignerItem.h"

class DesignerCanvas : public QWidget {
  Q_OBJECT
public:
  explicit DesignerCanvas(QWidget* parent = nullptr);
  ~DesignerCanvas() override = default;

  bool eventFilter(QObject *watched, QEvent *event) override;

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  void dropEvent(QDropEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  IDesignerItem *itemForObject(const QObject *watched) const;
  IDesignerItem::ResizeHandle resizeHandleForObject(const QObject *watched, QPoint globalPos) const;
  static QRect resizedGeometry(const QRect &start, IDesignerItem::ResizeHandle handle, QPoint delta);
  void selectItemsUnderRubberBand();
  void unselectAllItems();
  void handleItemMousePress(IDesignerItem *item, QWidget *widget, const QMouseEvent *event);
  void handleItemMouseMove(QMouseEvent *event);
  void handleItemMouseRelease();
  IDesignerItem *createItemFromJson(const QJsonObject& json);


  QSet<IDesignerItem*> m_selectedItems;
  QList<IDesignerItem*> m_items;

  QRubberBand *m_rubberBand = nullptr; // will create a custom class for it and create a better paint method
  QPointF m_rubberBandStartPos;
  QPointF m_rubberBandEndPos;

  QObject* m_clickedWidget = nullptr;

  QPointF m_dragStartPos;
  QHash<IDesignerItem*, QPoint> m_dragStartPositions;

  IDesignerItem::ResizeHandle m_resizeHandle = IDesignerItem::ResizeHandle::None;
  QWidget *m_resizeWidget = nullptr;
  QRect m_resizeStartGeometry;
  static constexpr QSize kMinItemSize{20, 20};

  bool m_isDragging = false;
  bool m_isResizing = false;
};
