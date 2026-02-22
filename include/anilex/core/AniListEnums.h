#pragma once
#include <QObject>


/***
 * Still figuring out how this works with ENUMS in Qt
***/
class AniList : public QObject {
    Q_OBJECT
public:
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
