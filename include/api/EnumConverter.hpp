#pragma once
#include <concepts>
#include <expected>
#include <QMetaObject>
#include <QMetaEnum>
#include <string>

#include "generated/anilist_enums.h"

template <typename T>
concept QtEnum = std::is_enum_v<T> && requires(T e) {
  { QtPrivate::qt_getEnumMetaObject(e) } -> std::same_as<const QMetaObject*>;
};

template <QtEnum T>
static std::string enumToString(T value) {
  QMetaEnum metaEnum = QMetaEnum::fromType<T>();
  if (!metaEnum.isValid()) return std::string();
  const char *key = metaEnum.valueToKey(static_cast<int>(value));
  if (!key) return std::string();
  return std::string(key);
}

template <QtEnum T>
T stringToEnum(std::string str) {
  QMetaEnum metaEnum = QMetaEnum::fromType<T>();
  bool ok = false;
  int value = metaEnum.keyToValue(str.c_str(), &ok);
  if (!ok) throw std::invalid_argument(std::string("Error converting to enum") + Q_FUNC_INFO);
  return static_cast<T>(value);
}