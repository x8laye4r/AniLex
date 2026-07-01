local text = "TEXT";
local date = "DATE_TEXT";
local scrollable = "SCROLLABLE";
local bool = "MEDIA_FLAGS";
local img = "IMAGE";

local path = "data.MediaListCollection.lists";
local fuzzyDate = "year month day";

local DateLabel(display, desc, fieldName, isFuzzy=false) = {
    displayName: display,
    type: date,
    desc: desc,
    dataSource: "%s.%s" % [path, fieldName],
    graphqlQuery: if isFuzzy then "%s { %s }" % [fieldName, fuzzyDate] else fieldName
};

local TextLabel(display, desc, fieldName, graphQL) = {
    displayName: display,
    type: text,
    desc: desc,
    dataSource: "%s.%s" % [path, fieldName],
    graphqlQuery: graphQL,
};

local Scrollable(display, desc, fieldName, graphQL, scroll = "vert") = {
    displayName: display,
    type: scrollable,
    desc: desc,
    dataSource: "%s.%s" % [path, fieldName],
    graphqlQuery: graphQL,
};

local MediaFlags(display, desc, fieldName, graphQL) = {
    displayName: display,
    type: bool,
    desc: desc,
    dataSource: "%s.%s" % [path, fieldName],
    graphqlQuery: graphQL,
};

local Image(display, desc, fieldName, graphQL) = {
    displayName: display,
    type: img,
    desc: desc,
    dataSource: "%s.%s" % [path, fieldName],
    graphqlQuery: graphQL
};

{
    Labels: {
        CompletedAtLabel: DateLabel("Completed At", "The date the user finished the entry", "completedAt", true),
        CreatedAtLabel:   DateLabel("Created At", "The date the entry was created", "createdAt", false),
        StartedAtLabel:   DateLabel("Started At", "The date the user started the entry", "startedAt", true),
        UpdatedAtLabel:   DateLabel("Updated At", "The date the entry was last updated", "updatedAt", false),
        EndDateLabel:     DateLabel("End Date", "The final official release date of the media", "endDate", true),
        StartDateLabel:   DateLabel("Start Date", "The first official release date of the media", "startDate", true),
        IdLabel:          TextLabel("Global ID", "The unique global identifier for the media", "id", "id"),
        MediaIdLabel:     TextLabel("Media ID", "The user-specific identifier for the media", "mediaId", "mediaId"),
        NotesLabel:       TextLabel("Notes", "The user's personal notes regarding the media", "notes", "notes"),
        PriorityLabel:    TextLabel("Priority", "The priority level set by the user", "priority", "priority"),
        RepeatLabel:      TextLabel("Repeats", "Number of times the user has repeated the media", "repeat", "repeat"),
        ScoreLabel:       TextLabel("Score", "The rating given by the user", "score", "score"),
        StatusLabel:      TextLabel("Status", "The current status of the media for the user", "status", "status"),
        AverageScoreLabel:TextLabel("Average Score", "The overall average score of the media", "media.averageScore", "averageScore"),
        CountryOfOriginLabel:TextLabel("Origin", "The country where the media originated", "media.countryOfOrigin", "countryOfOrigin"),
        DescriptionLabel: TextLabel("Description", "A brief overview or summary of the media", "media.description", "description"),
        DurationLabel:    TextLabel("Duration", "The total length of the media", "media.duration", "duration"),
        FavouritesLabel:  TextLabel("Favorites", "Total number of users who favorited the media", "media.favourites", "favourites"),
        FormatLabel:      TextLabel("Format", "The release format of the media", "media.format", "format"),
        FormatHashtag:    TextLabel("Hashtag", "The official social media hashtag", "media.hashtag", "hashtag"),
        MeanscoreLabel:   TextLabel("Mean Score", "The mean score calculated for the media", "media.meanscore", "meanscore"),
        PopularityLabel:  TextLabel("Popularity", "The popularity ranking of the media", "media.popularity", "popularity"),
        SeasonLabel:      TextLabel("Season", "The release season of the media", "media.season", "season"),
        SeasonYearLabel:  TextLabel("Season Year", "The year the release season occurred", "media.seasonYear", "seasonYear"),
        SourceLabel:      TextLabel("Source", "The original source material of the media", "media.source", "source"),
        TitleLabel:       TextLabel("Title", "The user's preferred title for the media", "media.title.userPreferred", "title { userPreferred }"),
        TrendingLabel:    TextLabel("Trending", "Current trending status based on posts per hour", "media.trending", "trending"),
        TypeLabel:        TextLabel("Type", "The category or type of the media", "media.type", "type"),
        NextAiringEpisodeLabel: TextLabel("Next Episode", "Information regarding the next scheduled episode", "media.nextAiringEpisode.timeUntilAiring", "nextAiringEpisode { timeUntilAiring }"),
        TrailerLabel:     TextLabel("Trailer", "Link to the official media trailer", "media.trailer.site", "trailer { site }")
    },

    Scrollareas: {
        AdvancedScoreScrollarea:    Scrollable("Advanced Scores", "Detailed user-specific scores", "advancedScores", "advancedScores"),
        CustomListsScrollarea:      Scrollable("Custom Lists", "Custom user lists containing this media", "customLists", "customLists"),
        AiringScheduleScrollarea:   Scrollable("Airing Schedule", "Release dates and times for all episodes", "media.airingSchedule.nodes.timeUntilAiring", "airingSchedule { nodes { timeUntilAiring episode } }"),
        ExternalLinksScrollarea:    Scrollable("External Links", "All associated external links for the media", "media.externalLinks.site", "externalLinks { site }"),
        GenresScrollarea:           Scrollable("Genres", "All genres associated with the media", "media.genres", "genres"),
        RankingsScrollarea:         Scrollable("Rankings", "Global and seasonal rankings of the media", "media.rankings.context;media.rankings.rank;media.rankings.season;media.rankings.year", "rankings { allTime context rank season year }"),
        RelationsScrollarea:        Scrollable("Relations", "All related media entries", "media.relations.nodes.title.userPreferred", "relations { nodes { id title { userPreferred } } }"),
        StaffScrollarea:            Scrollable("Staff", "The production staff of the media", "media.staff.nodes.name.full", "staff { nodes { name { full } } }"),
        ScoreDistributionScrollarea:Scrollable("Score Distribution", "The global distribution of user scores", "media.scoreStats.scoreDistribution.score;media.scoreStats.scoreDistribution.amount", "scoreStats: stats { scoreDistribution { amount score } }"),
        StatusDistributionScrollarea:Scrollable("Status Distribution", "The global distribution of media statuses", "media.statusStats.statusDistribution.status;media.statusStats.statusDistribution.amount", "statusStats: stats { statusDistribution { amount status } }"),
        StudiosScrollarea:          Scrollable("Studios", "The names of the involved production studios", "media.studios.nodes.name", "studios { nodes { name } }"),
        SynonymsScrollarea:         Scrollable("Synonyms", "Alternative titles for the media", "media.synonyms", "synonyms"),
        TagsScrollarea:             Scrollable("Tags", "Metadata tags associated with the media", "media.tags.name", "tags { name }")
    },

    MediaFlags: {
        HiddenFromStatusListIcon:   MediaFlags("Hidden", "Whether the media is hidden from the status list", "hiddenFromStatusLists", "hiddenFromStatusLists"),
        PrivateIcon:                MediaFlags("Private", "Whether the entry is hidden from public view", "private", "private"),
        IsAdultIcon:                MediaFlags("Adult", "Whether the media is classified as adult content", "media.isAdult", "isAdult"),
        IsFavoriteIcon:             MediaFlags("Favorite", "Whether the user has favorited this media", "media.isFavourite", "isFavourite"),
        IsLicensedIcon:             MediaFlags("Licensed", "Whether the media is officially licensed", "media.isLicensed", "isLicensed"),
        IsLockedIcon:               MediaFlags("Locked", "Whether the media entry is locked", "media.isLocked", "isLocked"),
        IsRecommendationLockIcon:   MediaFlags("Recommendation Blocked", "Whether the media is blocked for recommendations", "media.isRecommendationBlocked", "isRecommendationBlocked"),
        IsReviewBlockedIcon:        MediaFlags("Review Blocked", "Whether reviews are blocked for this media", "media.isReviewBlocked", "isReviewBlocked"),
    },

    Image: {
        CoverImage:                 Image("Cover Image", "The primary cover image of the media", "media.coverImage", "coverImage { extraLarge }"),
        BannerImage:                Image("Banner Image", "The background banner image of the media", "media.bannerImage", "bannerImage")
    }
}