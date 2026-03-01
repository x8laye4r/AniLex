#pragma once
#include <QObject>
#include <QVariant>

/***
 * Still figuring out how this works with ENUMS in Qt
***/
class AniListEnums : public QObject {
    Q_OBJECT
public:
    explicit AniListEnums(QObject *parent = nullptr) : QObject(parent) {}


    enum MediaStatus {
        CANCELLED,
        FINISHED,
        HIATUS,
        NOT_YET_RELEASED,
        RELEASING
    }; Q_ENUM(MediaStatus)

    enum MediaType {
        ANIME,
        MANGA
    }; Q_ENUM(MediaType)

    enum ReviewRating {
        DOWN_VOTE,
        NO_VOTE,
        UP_VOTE
    }; Q_ENUM(ReviewRating);
};
