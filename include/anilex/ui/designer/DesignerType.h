#pragma once
#include <QWidget>

class DesignerType {
  Q_GADGET
public:
  enum class DesignerItemType {
    TEXT,
    DATE_TEXT,
    BOOLEAN,
    IMAGE,
    SCROLLABLE,
    UNKNOWN
  }; Q_ENUM(DesignerItemType)

  enum class DesignerCreatorItems {
    ANIME_GENERAL,
    MANGA_GENERAL,

    ANIME_CURRENT,
    MANGA_CURRENT,
    ANIME_PLANNING,
    MANGA_PLANNING,
    ANIME_COMPLETED,
    MANGA_COMPLETED,
    ANIME_DROPPED,
    MANGA_DROPPED,
    ANIME_PAUSED,
    MANGA_PAUSED,
    ANIME_REPEATING,
    MANGA_REPEATING
  }; Q_ENUM(DesignerCreatorItems)

};
