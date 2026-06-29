#include "anilex/ui/designer/DesignerWidgetListItem.h"

DesignerWidgetListItem::DesignerWidgetListItem(const QString &title, QWidget *parent)
  : Section(title, 100, parent) {}

void DesignerWidgetListItem::setWidgets(QStringList &widgetNames) {
  for (const QString &widgetName : widgetNames) {
    
  }
}
