#pragma once
#include <QObject>
#include <QVariant>

/**
 * @brief Still figuring out how this works with ENUMS in Qt
*/
class AniListEnums : public QObject {
    Q_OBJECT
public:
    explicit AniListEnums(QObject *parent = nullptr) : QObject(parent) {}

    enum class MediaStatus {
        CANCELLED,
        FINISHED,
        HIATUS,
        NOT_YET_RELEASED,
        RELEASING
    }; Q_ENUM(MediaStatus)

    enum class MediaType {
        ANIME,
        MANGA
    }; Q_ENUM(MediaType)

    enum class ReviewRating {
        DOWN_VOTE,
        NO_VOTE,
        UP_VOTE
    }; Q_ENUM(ReviewRating);
};
