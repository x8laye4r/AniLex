#pragma once
#include <QWidget>
#include <QRubberBand>
#include "IDesignerItem.h"

class DesignerCanvas : public QWidget {
  Q_OBJECT
public:
  explicit DesignerCanvas(QWidget* parent = nullptr);
  ~DesignerCanvas() override = default;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  void dropEvent(QDropEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
private:
  QSet<IDesignerItem*> m_selectedItems;
  QList<IDesignerItem*> m_items;

  QRubberBand *rubberBand = nullptr;

  QPointF m_startDragPos;
  QPointF m_dragPos;

  bool m_isDragging;
  bool m_isResizing;
};
