#pragma once
#include <QGraphicsView>

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
private:
  QGraphicsScene *m_scene = nullptr;
};
