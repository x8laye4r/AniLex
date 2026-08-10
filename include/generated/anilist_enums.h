#pragma once
#include <QObject>

/*
 * Auto Generated File via a python script
 * Do not Edit manually
 */

namespace anilist_enums {
    Q_NAMESPACE

    enum class ActivitySort {
        ID,
        ID_DESC,
        PINNED
    };
    Q_ENUM_NS(ActivitySort)

    enum class ActivityType {
        ANIME_LIST,
        MANGA_LIST,
        MEDIA_LIST,
        MESSAGE,
        TEXT
    };
    Q_ENUM_NS(ActivityType)

    enum class AiringSort {
        EPISODE,
        EPISODE_DESC,
        ID,
        ID_DESC,
        MEDIA_ID,
        MEDIA_ID_DESC,
        TIME,
        TIME_DESC
    };
    Q_ENUM_NS(AiringSort)

    enum class CharacterRole {
        BACKGROUND,
        MAIN,
        SUPPORTING
    };
    Q_ENUM_NS(CharacterRole)

    enum class CharacterSort {
        FAVOURITES,
        FAVOURITES_DESC,
        ID,
        ID_DESC,
        RELEVANCE,
        ROLE,
        ROLE_DESC,
        SEARCH_MATCH
    };
    Q_ENUM_NS(CharacterSort)

    enum class ExternalLinkMediaType {
        ANIME,
        MANGA,
        STAFF
    };
    Q_ENUM_NS(ExternalLinkMediaType)

    enum class ExternalLinkType {
        INFO,
        SOCIAL,
        STREAMING
    };
    Q_ENUM_NS(ExternalLinkType)

    enum class LikeableType {
        ACTIVITY,
        ACTIVITY_REPLY,
        THREAD,
        THREAD_COMMENT
    };
    Q_ENUM_NS(LikeableType)

    enum class MediaFormat {
        MANGA,
        MOVIE,
        MUSIC,
        NOVEL,
        ONA,
        ONE_SHOT,
        OVA,
        SPECIAL,
        TV,
        TV_SHORT
    };
    Q_ENUM_NS(MediaFormat)

    enum class MediaListSort {
        ADDED_TIME,
        ADDED_TIME_DESC,
        FINISHED_ON,
        FINISHED_ON_DESC,
        MEDIA_ID,
        MEDIA_ID_DESC,
        MEDIA_POPULARITY,
        MEDIA_POPULARITY_DESC,
        MEDIA_TITLE_ENGLISH,
        MEDIA_TITLE_ENGLISH_DESC,
        MEDIA_TITLE_NATIVE,
        MEDIA_TITLE_NATIVE_DESC,
        MEDIA_TITLE_ROMAJI,
        MEDIA_TITLE_ROMAJI_DESC,
        PRIORITY,
        PRIORITY_DESC,
        PROGRESS,
        PROGRESS_DESC,
        PROGRESS_VOLUMES,
        PROGRESS_VOLUMES_DESC,
        REPEAT,
        REPEAT_DESC,
        SCORE,
        SCORE_DESC,
        STARTED_ON,
        STARTED_ON_DESC,
        STATUS,
        STATUS_DESC,
        UPDATED_TIME,
        UPDATED_TIME_DESC
    };
    Q_ENUM_NS(MediaListSort)

    enum class MediaListStatus {
        COMPLETED,
        CURRENT,
        DROPPED,
        PAUSED,
        PLANNING,
        REPEATING
    };
    Q_ENUM_NS(MediaListStatus)

    enum class MediaRankType {
        POPULAR,
        RATED
    };
    Q_ENUM_NS(MediaRankType)

    enum class MediaRelation {
        ADAPTATION,
        ALTERNATIVE,
        CHARACTER,
        COMPILATION,
        CONTAINS,
        OTHER,
        PARENT,
        PREQUEL,
        SAME_UNIVERSE,
        SEQUEL,
        SIDE_STORY,
        SOURCE,
        SPIN_OFF,
        SUMMARY
    };
    Q_ENUM_NS(MediaRelation)

    enum class MediaSeason {
        FALL,
        SPRING,
        SUMMER,
        WINTER
    };
    Q_ENUM_NS(MediaSeason)

    enum class MediaSort {
        CHAPTERS,
        CHAPTERS_DESC,
        DURATION,
        DURATION_DESC,
        END_DATE,
        END_DATE_DESC,
        EPISODES,
        EPISODES_DESC,
        FAVOURITES,
        FAVOURITES_DESC,
        FORMAT,
        FORMAT_DESC,
        ID,
        ID_DESC,
        POPULARITY,
        POPULARITY_DESC,
        SCORE,
        SCORE_DESC,
        SEARCH_MATCH,
        START_DATE,
        START_DATE_DESC,
        STATUS,
        STATUS_DESC,
        TITLE_ENGLISH,
        TITLE_ENGLISH_DESC,
        TITLE_NATIVE,
        TITLE_NATIVE_DESC,
        TITLE_ROMAJI,
        TITLE_ROMAJI_DESC,
        TRENDING,
        TRENDING_DESC,
        TYPE,
        TYPE_DESC,
        UPDATED_AT,
        UPDATED_AT_DESC,
        VOLUMES,
        VOLUMES_DESC
    };
    Q_ENUM_NS(MediaSort)

    enum class MediaSource {
        ANIME,
        COMIC,
        DOUJINSHI,
        GAME,
        LIGHT_NOVEL,
        LIVE_ACTION,
        MANGA,
        MULTIMEDIA_PROJECT,
        NOVEL,
        ORIGINAL,
        OTHER,
        PICTURE_BOOK,
        VIDEO_GAME,
        VISUAL_NOVEL,
        WEB_NOVEL
    };
    Q_ENUM_NS(MediaSource)

    enum class MediaStatus {
        CANCELLED,
        FINISHED,
        HIATUS,
        NOT_YET_RELEASED,
        RELEASING
    };
    Q_ENUM_NS(MediaStatus)

    enum class MediaTrendSort {
        DATE,
        DATE_DESC,
        EPISODE,
        EPISODE_DESC,
        ID,
        ID_DESC,
        MEDIA_ID,
        MEDIA_ID_DESC,
        POPULARITY,
        POPULARITY_DESC,
        SCORE,
        SCORE_DESC,
        TRENDING,
        TRENDING_DESC
    };
    Q_ENUM_NS(MediaTrendSort)

    enum class MediaType {
        ANIME,
        MANGA
    };
    Q_ENUM_NS(MediaType)

    enum class ModActionType {
        ANON,
        BAN,
        DELETE,
        EDIT,
        EXPIRE,
        NOTE,
        REPORT,
        RESET
    };
    Q_ENUM_NS(ModActionType)

    enum class ModRole {
        ADMIN,
        ANIME_DATA,
        CHARACTER_DATA,
        COMMUNITY,
        DEVELOPER,
        DISCORD_COMMUNITY,
        LEAD_ANIME_DATA,
        LEAD_COMMUNITY,
        LEAD_DEVELOPER,
        LEAD_MANGA_DATA,
        LEAD_SOCIAL_MEDIA,
        MANGA_DATA,
        RETIRED,
        SOCIAL_MEDIA,
        STAFF_DATA
    };
    Q_ENUM_NS(ModRole)

    enum class NotificationType {
        ACTIVITY_LIKE,
        ACTIVITY_MENTION,
        ACTIVITY_MESSAGE,
        ACTIVITY_REPLY,
        ACTIVITY_REPLY_LIKE,
        ACTIVITY_REPLY_SUBSCRIBED,
        AIRING,
        CHARACTER_SUBMISSION_UPDATE,
        FOLLOWING,
        MEDIA_DATA_CHANGE,
        MEDIA_DELETION,
        MEDIA_MERGE,
        MEDIA_SUBMISSION_UPDATE,
        RELATED_MEDIA_ADDITION,
        STAFF_SUBMISSION_UPDATE,
        THREAD_COMMENT_LIKE,
        THREAD_COMMENT_MENTION,
        THREAD_COMMENT_REPLY,
        THREAD_LIKE,
        THREAD_SUBSCRIBED
    };
    Q_ENUM_NS(NotificationType)

    enum class RecommendationRating {
        NO_RATING,
        RATE_DOWN,
        RATE_UP
    };
    Q_ENUM_NS(RecommendationRating)

    enum class RecommendationSort {
        ID,
        ID_DESC,
        RATING,
        RATING_DESC
    };
    Q_ENUM_NS(RecommendationSort)

    enum class ReviewRating {
        DOWN_VOTE,
        NO_VOTE,
        UP_VOTE
    };
    Q_ENUM_NS(ReviewRating)

    enum class ReviewSort {
        CREATED_AT,
        CREATED_AT_DESC,
        ID,
        ID_DESC,
        RATING,
        RATING_DESC,
        SCORE,
        SCORE_DESC,
        UPDATED_AT,
        UPDATED_AT_DESC
    };
    Q_ENUM_NS(ReviewSort)

    enum class RevisionHistoryAction {
        CREATE,
        EDIT
    };
    Q_ENUM_NS(RevisionHistoryAction)

    enum class ScoreFormat {
        POINT_10,
        POINT_100,
        POINT_10_DECIMAL,
        POINT_3,
        POINT_5
    };
    Q_ENUM_NS(ScoreFormat)

    enum class SiteTrendSort {
        CHANGE,
        CHANGE_DESC,
        COUNT,
        COUNT_DESC,
        DATE,
        DATE_DESC
    };
    Q_ENUM_NS(SiteTrendSort)

    enum class StaffLanguage {
        ENGLISH,
        FRENCH,
        GERMAN,
        HEBREW,
        HUNGARIAN,
        ITALIAN,
        JAPANESE,
        KOREAN,
        PORTUGUESE,
        SPANISH
    };
    Q_ENUM_NS(StaffLanguage)

    enum class StaffSort {
        FAVOURITES,
        FAVOURITES_DESC,
        ID,
        ID_DESC,
        LANGUAGE,
        LANGUAGE_DESC,
        RELEVANCE,
        ROLE,
        ROLE_DESC,
        SEARCH_MATCH
    };
    Q_ENUM_NS(StaffSort)

    enum class StudioSort {
        FAVOURITES,
        FAVOURITES_DESC,
        ID,
        ID_DESC,
        NAME,
        NAME_DESC,
        SEARCH_MATCH
    };
    Q_ENUM_NS(StudioSort)

    enum class SubmissionSort {
        ID,
        ID_DESC
    };
    Q_ENUM_NS(SubmissionSort)

    enum class SubmissionStatus {
        ACCEPTED,
        PARTIALLY_ACCEPTED,
        PENDING,
        REJECTED
    };
    Q_ENUM_NS(SubmissionStatus)

    enum class ThreadCommentSort {
        ID,
        ID_DESC
    };
    Q_ENUM_NS(ThreadCommentSort)

    enum class ThreadSort {
        CREATED_AT,
        CREATED_AT_DESC,
        ID,
        ID_DESC,
        IS_STICKY,
        REPLIED_AT,
        REPLIED_AT_DESC,
        REPLY_COUNT,
        REPLY_COUNT_DESC,
        SEARCH_MATCH,
        TITLE,
        TITLE_DESC,
        UPDATED_AT,
        UPDATED_AT_DESC,
        VIEW_COUNT,
        VIEW_COUNT_DESC
    };
    Q_ENUM_NS(ThreadSort)

    enum class UserSort {
        CHAPTERS_READ,
        CHAPTERS_READ_DESC,
        ID,
        ID_DESC,
        SEARCH_MATCH,
        USERNAME,
        USERNAME_DESC,
        WATCHED_TIME,
        WATCHED_TIME_DESC
    };
    Q_ENUM_NS(UserSort)

    enum class UserStaffNameLanguage {
        NATIVE,
        ROMAJI,
        ROMAJI_WESTERN
    };
    Q_ENUM_NS(UserStaffNameLanguage)

    enum class UserStatisticsSort {
        COUNT,
        COUNT_DESC,
        ID,
        ID_DESC,
        MEAN_SCORE,
        MEAN_SCORE_DESC,
        PROGRESS,
        PROGRESS_DESC
    };
    Q_ENUM_NS(UserStatisticsSort)

    enum class UserTitleLanguage {
        ENGLISH,
        ENGLISH_STYLISED,
        NATIVE,
        NATIVE_STYLISED,
        ROMAJI,
        ROMAJI_STYLISED
    };
    Q_ENUM_NS(UserTitleLanguage)

} // namespace anilist_enums