#pragma once
#include <QLabel>

#include "DesignerType.h"

class DesignerWidgetListItem : public QLabel {
  Q_OBJECT
public:
  explicit DesignerWidgetListItem(const QString &text, DesignerType::DesignerItemType type, QWidget *parent = nullptr);
protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
private:
  QPoint m_dragStartPos;
  DesignerType::DesignerItemType m_designerType;
};
