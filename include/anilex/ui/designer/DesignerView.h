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
private:
  QGraphicsScene *m_scene = nullptr;
};
