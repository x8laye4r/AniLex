#pragma once
#include <QScrollArea>
#include <QVBoxLayout>

#include "anilex/ui/interfaces/AbstractDesignerItem.h"

struct EditorComponent {
  QWidget *widget = nullptr;
  std::function<void(const QVariant&)> updater;
};

using EditorCreator = std::function<EditorComponent(QWidget *parent, const QVariant& value, const std::function<void(const QVariant&)> &setter)>;

class DesignerPropertyEditor : public QScrollArea {
  Q_OBJECT
  public:
  explicit DesignerPropertyEditor(QWidget* parent = nullptr);
public slots:
  void widgetChanged(AbstractDesignerItem *item);
  void clearEditor(QLayout* layoutToClear = nullptr);

private:
  AbstractDesignerItem* m_selectedItem = nullptr;

  QVBoxLayout* m_scrollLayout = nullptr;
  QMap<int, EditorCreator> m_editorRegistries;
  QMap<int, std::function<void(const QVariant&)>> m_propertyUpdaters;

  void createEditorWidget(QList<QMetaProperty> &properties);
  void initRegistry();
  EditorComponent createEnumCreator(const QMetaProperty &property, const QVariant &value, auto setter);
};
