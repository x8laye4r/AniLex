#pragma once
#include <QMap>
#include <QString>
#include <functional>
#include <iostream>
#include <memory>
#include "IDesignerItem.h"

using DesignerItemCreator = std::function<std::unique_ptr<IDesignerItem>(QWidget*)>;

class DesignerItemFactory {
public:
  static DesignerItemFactory& instance() {
    static DesignerItemFactory itemFactory;
    return itemFactory;
  }

  void registerType(const QString &name, const DesignerItemCreator& creator) {
    m_designerTypes[name] = creator;
  }

  std::unique_ptr<IDesignerItem> getItem(const QString &name, QWidget *parent = nullptr) const {
    if (!m_designerTypes.contains(name)) return nullptr;
    return m_designerTypes[name](parent);
  }

  void printAll() {
    for (const QString &key : m_designerTypes.keys()) {
      std::cout << key.toStdString() << std::endl;
    }
  }

  DesignerItemFactory(const DesignerItemFactory&) = delete;
  DesignerItemFactory& operator=(const DesignerItemFactory&) = delete;
  DesignerItemFactory(DesignerItemFactory&&) = delete;
  DesignerItemFactory& operator=(DesignerItemFactory&&) = delete;

private:
  QMap<QString, DesignerItemCreator> m_designerTypes;
  DesignerItemFactory() = default;
};

template <typename T>
class DesignerRegistrar {
public:
  explicit DesignerRegistrar(const QString &name) {
    DesignerItemFactory::instance().registerType(name, [](QWidget *parent) {
        return std::make_unique<T>(parent);
    });
  }
};

#define REGISTER_DESIGNER_ITEM(ClassName, RegistryName) \
static DesignerRegistrar<ClassName> designer_item_registry_##ClassName(RegistryName);