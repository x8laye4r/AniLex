#pragma once
#include <concepts>
#include <QMetaObject>
#include <QMetaEnum>
#include <string>

template <typename T>
concept QtEnum = std::is_enum_v<T> && requires {
  { qt_getEnumMetaObject(T{}) } -> std::same_as<const QMetaObject*>;
  { qt_getEnumName(T{}) } -> std::same_as<const char*>;
};


template <QtEnum T>
static std::string enumToString(const T value) {
  const QMetaEnum metaEnum = QMetaEnum::fromType<T>();

  if (!metaEnum.isValid()) {
    return std::string{};
  }

  const char *key = metaEnum.valueToKey(static_cast<int>(value));

  if (!key) {
    return std::string{};
  }

  return std::string{key};
}

template <QtEnum T>
T stringToEnum(const std::string& str) {
  const QMetaEnum metaEnum = QMetaEnum::fromType<T>();

  bool ok = false;

  int value = metaEnum.keyToValue(str.c_str(), &ok);

  if (!ok) {
    throw std::invalid_argument(Q_FUNC_INFO + std::string("Error converting to enum: "));
  }

  return static_cast<T>(value);
}