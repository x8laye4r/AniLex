#include "anilex/ui/designer/DesignerPropertyEditor.h"

#include <qmetaobject.h>

DesignerPropertyEditor::DesignerPropertyEditor(QWidget *parent)
  : QScrollArea(parent) {
  this->setWidgetResizable(true);

  m_scrollAreaContainerWidget = new QWidget(this);
  m_scrollAreaContainerWidgetLayout = new QVBoxLayout(m_scrollAreaContainerWidget);
  m_scrollAreaContainerWidget->setLayout(m_scrollAreaContainerWidgetLayout);

  this->setWidget(m_scrollAreaContainerWidget);
}

void DesignerPropertyEditor::initEditorCreators() {
  m_editorCreators[QMetaType::Int] = []() {

  };
}

void DesignerPropertyEditor::onWidgetChanged(IDesignerItem *item) {
  if (item == nullptr) return;

  this->clearEditor();
  const QMetaObject *metaObject = item->asWidget()->metaObject();

  QList<QMetaProperty> propertyList;

  const int propertyCount = metaObject->propertyCount();
  const int propertyOffset = metaObject->propertyOffset();

  for (int i = propertyOffset; i < propertyCount; i++) {
    if (metaObject->property(i).isWritable()) {
      propertyList.append(metaObject->property(i));
    }
  }

}

void DesignerPropertyEditor::clearEditor(QLayout *layout) {
}

void DesignerPropertyEditor::insertWidgetsIntoEditor(QList<QMetaProperty> &propertyList) {
}
