#pragma once
#include <functional>
#include <QScrollArea>
#include <QMetaProperty>
#include <QVBoxLayout>

#include "IDesignerItem.h"

using EditorCreator = std::function<void()>;

class DesignerPropertyEditor : public QScrollArea {
public:
  explicit DesignerPropertyEditor(QWidget *parent = nullptr);

public slots:
  void onWidgetChanged(IDesignerItem *item);

private:
  void clearEditor(QLayout *layout = nullptr);
  void insertWidgetsIntoEditor(QList<QMetaProperty> &propertyList);
  void initEditorCreators();

  QWidget *m_scrollAreaContainerWidget = nullptr;
  QVBoxLayout *m_scrollAreaContainerWidgetLayout = nullptr;

  QMap<QMetaType::Type, EditorCreator> m_editorCreators;
};
