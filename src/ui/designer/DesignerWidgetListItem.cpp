#include "anilex/ui/designer/DesignerWidgetListItem.h"
#include "anilex/ui/designer/IDesignerItem.h"
#include <QCoreApplication>

#include "anilex/ui/designer/DesignerDragableLabel.h"

DesignerWidgetListItem::DesignerWidgetListItem(QWidget *parent)
  : Section("", 100, parent) {}

void DesignerWidgetListItem::setWidget(DesignerCategory category) {
  const QString translatedCategoryName = TRANSLATE(category.categoryTitle.toUtf8().constData());

  QVBoxLayout *layout = new QVBoxLayout(); // todo: need to see if this is a memory leak

  this->setTitle(translatedCategoryName);
  for (DesignerItem item : category.items) {
    const QString trans_label_text = TRANSLATE(item.displayName.toUtf8().constData());
    // const QString trans_toolTip_text = TRANSLATE(item.toolTip.toUtf8().constData());

    DesignerDragableLabel *label = new DesignerDragableLabel(trans_label_text, item, this);
    layout->addWidget(label);
  }

  this->setContentLayout(*layout);
}