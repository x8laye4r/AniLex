#pragma once
#include <QGraphicsView>
#include "anilex/ui/interfaces/AbstractDesignerItem.h"

class DesignerView : public QGraphicsView {
  Q_OBJECT
public:
  explicit DesignerView(QWidget* parent = nullptr);

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;
  bool hasHeightForWidth() const override;
  int heightForWidth(int width) const override;
protected:
  void dropEvent(QDropEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
private:
  QGraphicsScene *m_scene = nullptr;
};
