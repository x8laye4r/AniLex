#pragma once
#include <QGraphicsView>

class DesignerView : public QGraphicsView {
  Q_OBJECT
public:
  explicit DesignerView(QWidget* parent = nullptr);
};
