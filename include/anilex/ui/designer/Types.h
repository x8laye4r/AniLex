#pragma once
#include <QObject>

namespace anilex_designer_helpers {
  Q_NAMESPACE

  enum class Types {
    NONE,
    TEXT,
    DATE_TEXT,
    SCROLLABLE,
    MEDIA_FLAGS,
    IMAGE
  }; Q_ENUM_NS(Types)
} // anilex_designer_helpers namespace