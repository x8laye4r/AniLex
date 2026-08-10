add_library(anilex_sources SHARED
        # sources
        main.cpp


        # includes
        ../include/api/GraphQLOperation.hpp
        ../include/generated/anilist_enums.h
)

target_include_directories(anilex_sources PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(anilex_sources PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Svg
        Qt6Keychain::Qt6Keychain
)

qt_add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE anilex_sources)

qt_add_resources(${PROJECT_NAME} "graphql_queries"
        PREFIX "/graphql"
        BASE "${CMAKE_SOURCE_DIR}/src/api"
        FILES
        ./api/operations/profile/FollowerListQuery.graphql
        ./api/operations/profile/ToggleFollowMutation.graphql
        ./api/operations/profile/FollowingListQuery.graphql
        ./api/operations/favorite/GetUserFavoriteCharacterQuery.graphql
        ./api/operations/favorite/ToggleFavoriteMutation.graphql
        ./api/operations/favorite/GetUserFavoriteMangaQuery.graphql
        ./api/operations/favorite/GetUserFavoriteStudioQuery.graphql
        ./api/operations/favorite/GetUserFavoriteAnimeQuery.graphql
        ./api/operations/favorite/GetUserFavoriteStaffQuery.graphql
        ./api/operations/activity/SaveMessageActivityMutation.graphql
        ./api/operations/activity/ToggleActivityPinMutation.graphql
        ./api/operations/activity/DeleteActivityMutation.graphql
        ./api/operations/activity/SaveActivityReplyMutation.graphql
        ./api/operations/activity/ToggleActivitySubscriptionMutation.graphql
        ./api/operations/activity/ActivityDetailsQuery.graphql
        ./api/operations/activity/ActivityFeedQuery.graphql
        ./api/operations/activity/DeleteActivityReplyMutation.graphql
        ./api/operations/activity/SaveTextActivityMutation.graphql
        ./api/operations/viewer/GetViewerQuery.graphql
        ./api/operations/GenreTagCollectionQuery.graphql
        ./api/operations/media/SearchMangaQuery.graphql
        ./api/operations/media/GetMediaListCollectionQuery.graphql
        ./api/operations/media/GetMediaRecomRelatQuery.graphql
        ./api/operations/media/SaveMediaListMutation.graphql
        ./api/operations/media/DeleteMediaListMutation.graphql
        ./api/operations/media/GetMediaDetailsQuery.graphql
        ./api/operations/media/GetMediaCharactersQuery.graphql
        ./api/operations/media/SearchAnimeQuery.graphql
        ./api/operations/media/GetMediaStaffQuery.graphql
        ./api/operations/media/GetMediaStatsQuery.graphql
        ./api/operations/studio/SearchStudioQuery.graphql
        ./api/operations/studio/GetStudioInfoQuery.graphql
        ./api/operations/MediaTagCollectionQuery.graphql
        ./api/operations/character/SearchCharacterQuery.graphql
        ./api/operations/character/GetCharacterDetailsQuery.graphql
        ./api/operations/review/GetReviewsQuery.graphql
        ./api/operations/review/RateReviewMutation.graphql
        ./api/operations/review/GetReviewDetailsQuery.graphql
        ./api/operations/ToggleLikeMutation.graphql
        ./api/operations/user/DeleteCustomListMutation.graphql
        ./api/operations/user/stats/GetUserStatsQuery.graphql
        ./api/operations/user/stats/GetVoiceActorsAnimeQuery.graphql
        ./api/operations/user/stats/GetAnimeOverviewQuery.graphql
        ./api/operations/user/stats/GetStaffMangaQuery.graphql
        ./api/operations/user/stats/GetMangaOverviewQuery.graphql
        ./api/operations/user/stats/GetStaffAnimeQuery.graphql
        ./api/operations/user/stats/GetStudiosQuery.graphql
        ./api/operations/user/GetUserNotificationsQuery.graphql
        ./api/operations/user/GetUnreadNotificationsQuery.graphql
        ./api/operations/user/CreateCustomListMutation.graphql
        ./api/operations/user/GetUserOptionsQuery.graphql
        ./api/operations/user/SearchUserQuery.graphql
        ./api/operations/user/GetUserProfileQuery.graphql
        ./api/operations/old_queries/DeleteActivityMutation.graphql
        ./api/operations/old_queries/ActivityUpdateMutation.graphql
        ./api/operations/old_queries/DeleteActivityReplyMutation.graphql
        ./api/operations/old_queries/ActivityUpdateReplyMutation.graphql
        ./api/operations/thread/DeleteThreadCommentMutation.graphql
        ./api/operations/thread/UpdateThreadMutation.graphql
        ./api/operations/thread/ToggleThreadSubscriptionMutation.graphql
        ./api/operations/thread/DeleteThreadMutation.graphql
        ./api/operations/thread/GetThreadCommentsQuery.graphql
        ./api/operations/thread/GetThreadQuery.graphql
        ./api/operations/staff/GetStaffMediaQuery.graphql
        ./api/operations/staff/GetStaffDetailsQuery.graphql
        ./api/operations/staff/GetStaffCharactersQuery.graphql
        ./api/operations/staff/SearchStaffQuery.graphql
        ./api/operations/home/GetPopularMoreQuery.graphql
        ./api/operations/home/GetNewMoreQuery.graphql
        ./api/operations/home/GetSeasonMoreQuery.graphql
        ./api/operations/home/GetReleasingMoreQuery.graphql
        ./api/operations/home/GetHomeScreenQuery.graphql
        ./api/operations/home/GetTrendingMoreQuery.graphql
        ./api/operations/home/GetAiringScheduleQuery.graphql
)