#pragma once
#include <QLabel>
#include "IDesignerItem.h"
using namespace anilex_designer_helpers;

class QDrag;

class DesignerDragableLabel : public QLabel {
  Q_OBJECT
public:
  explicit DesignerDragableLabel(const QString &text, DesignerItem item, QWidget *parent = nullptr);
  ~DesignerDragableLabel() override = default;

protected:
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

private:
  QDrag* createDrag();

  QPointF m_dragStartPos;
  DesignerItem m_properties;
};

