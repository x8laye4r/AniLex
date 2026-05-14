#pragma once
#include <QWidget>

class DesignerType {
  Q_GADGET
public:
  enum DesignerItemType {
    TEXT,
    DATE_TEXT,
    BOOLEAN,
    IMAGE,
    SCROLLABLE
  }; Q_ENUM(DesignerItemType)
};
