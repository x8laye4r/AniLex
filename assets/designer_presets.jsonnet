local text = "TEXT";
local date = "DATE_TEXT";
local scrollable = "SCROLLABLE";
local bool = "BOOLEAN";
local img = "IMAGE";

local fontStandard = "color: white; font-size: 12;";
local path = "data.MediaListCollection.lists";
local fuzzyDate = "year month day";

local DateLabel(desc, fieldName, isFuzzy=false) = {
    type: date,
    role: "text.date",
    desc: desc,
    dataSource: "%s.%s" % [path, fieldName],
    graphQL: if isFuzzy then "%s { %s }" % [fieldName, fuzzyDate] else fieldName
};

local TextLabel(desc, fieldName, graphQL, style=fontStandard, animeOnly=false) = {
    type: text,
    role: "text",
    desc: desc,
    dataSource: "%s.%s" % [path, fieldName],
    graphQL: graphQL,
    animeOnly: animeOnly
};

local Scrollable(desc, fieldName, graphQL, scroll = "vert", animeOnly=false) = {
    type: scrollable,
    role: "scroll.%s" % [scroll],
    desc: desc,
    dataSource: "%s.%s" % [path, fieldName],
    graphQL: graphQL,
    animeOnly: animeOnly
};

local Boolean(desc, fieldName, graphQL) = {
    type: bool,
    role: "bool",
    desc: desc,
    dataSource: "%s.%s" % [path, fieldName],
    graphQL: graphQL,
};

local Image(desc, fieldName, graphQL, banner=false) = {
    type: img,
    role: "img.%s" % [if banner then "banner" else "cover"],
    desc: desc,
    dataSource: "%s.%s" % [path, fieldName],
    graphQL: graphQL
};

{
    CompletedAtLabel: DateLabel("When the user finished the entry", "completedAt", true),
    CreatedAtLabel:   DateLabel("When the user created the entry", "createdAt", false),
    StartedAtLabel:   DateLabel("When the user started the entry", "startedAt", true),
    UpdatedAtLabel:   DateLabel("When the user updated the entry", "updatedAt", false),
    EndDateLabel:     DateLabel("Last official release of the media", "endDate", true),
    StartDateLabel:   DateLabel("First official release of the media", "startDate", true),
    IdLabel:          TextLabel("The global id of the media", "id", "id"),
    MediaIdLabel:     TextLabel("The users id of the media", "mediaId", "mediaId"),
    NotesLabel:       TextLabel("The users note of the media", "notes", "notes"),
    PriorityLabel:    TextLabel("Priority the user set for the media", "priority", "priority"),
    RepeatLabel:      TextLabel("How often the user repeated the media", "repeat", "repeat"),
    ScoreLabel:       TextLabel("How the user rated the media", "score", "score"),
    StatusLabel:      TextLabel("Which status the media has for the user", "status", "status"),
    AverageScoreLabel:TextLabel("The average score of the media", "media.averageScore", "averageScore"),
    CountryOfOriginLabel:TextLabel("Where the media is from", "media.countryOfOrigin", "countryOfOrigin"),
    DescriptionLabel: TextLabel("The description of the media", "media.description", "description"),
    DurationLabel:    TextLabel("Duration of the media", "media.duration", "duration"),
    FavouritesLabel:  TextLabel("How many favorites the media has", "media.favourites", "favourites"),
    FormatLabel:      TextLabel("Which format the media has", "media.format", "format"),
    FormatHashtag:    TextLabel("Official Twitter/X Hashtag", "media.hashtag", "hashtag"),
    MeanscoreLabel:   TextLabel("The mean score of the media", "media.meanscore", "meanscore"),
    PopularityLabel:  TextLabel("Popularity of the media", "media.popularity", "popularity"),
    SeasonLabel:      TextLabel("The season the media released", "media.season", "season"),
    SeasonYearLabel:  TextLabel("The year the season was released", "media.seasonYear", "seasonYear"),
    SourceLabel:      TextLabel("The source of the media", "media.source", "source"),
    TitleLabel:       TextLabel("The title of the media", "media.title.userPreferred", "title { userPreferred }"),
    TrendingLabel:    TextLabel("How much the media is trending (posts per hour)", "media.trending", "trending"),
    TypeLabel:        TextLabel("What type the media is", "media.type", "type"),
    NextAiringEpisodeLabel: TextLabel("The next airing episode of the Anime", "media.nextAiringEpisode.timeUntilAiring", "nextAiringEpisode { timeUntilAiring }", true),
    TrailerLabel:     TextLabel("Trailer of the media (link only)", "media.trailer.site", "trailer { site }"),
    AdvancedScoreScrollarea:    Scrollable("Your advanced scores of the media", "advancedScores", "advancedScores"),
    CustomListsScrollarea:      Scrollable("Custom lists the media is in", "customLists", "customLists"),
    AiringScheduleScrollarea:   Scrollable("The release dates of all episodes", "media.airingSchedule.nodes.timeUntilAiring", "airingSchedule { nodes { timeUntilAiring episode } }"),
    ExternalLinksScrollarea:    Scrollable("All the external links of the media", "media.externalLinks.site", "externalLinks { site }"),
    GenresScrollarea:           Scrollable("All genres of the media", "media.genres", "genres"),
    RankingsScrollarea:         Scrollable("All the rankings of the media", "media.rankings.context;media.rankings.rank;media.rankings.season;media.rankings.year", "rankings { allTime context rank season year }"),
    RelationsScrollarea:        Scrollable("All the relations of the media", "media.relations.nodes.title.userPreferred", "relations { nodes { id title { userPreferred } } }"),
    StaffScrollarea:            Scrollable("The staff of the media (names only)", "media.staff.nodes.name.full", "staff { nodes { name { full } } }"),
    ScoreDistributionScrollarea:Scrollable("The distribution of the score of the media", "media.scoreStats.scoreDistribution.score;media.scoreStats.scoreDistribution.amount", "scoreStats: stats { scoreDistribution { amount score } }"),
    StatusDistributionScrollarea:Scrollable("The distribution of the status of the media", "media.statusStats.statusDistribution.status;media.statusStats.statusDistribution.amount", "statusStats: stats { statusDistribution { amount status } }"),
    StudiosScrollarea:          Scrollable("The names of the studios", "media.studios.nodes.name", "studios { nodes { name } }"),
    SynonymsScrollarea:         Scrollable("The synonyms of the media", "media.synonyms", "synonyms"),
    TagsScrollarea:             Scrollable("The Tags of the media", "media.tags.name", "tags { name }"),
    HiddenFromStatusListIcon:   Boolean("If the media is hidden from status list", "hiddenFromStatusLists", "hiddenFromStatusLists"),
    PrivateIcon:                Boolean("If the media is set hidden from the public", "private", "private"),
    IsAdultIcon:                Boolean("If the media is adult", "media.isAdult", "isAdult"),
    IsFavoriteIcon:             Boolean("If the media is favorited by the user", "media.isFavourite", "isFavourite"),
    IsLicensedIcon:             Boolean("If the media is somewhere licensed", "media.isLicensed", "isLicensed"),
    IsLockedIcon:               Boolean("If the media is locked", "media.isLocked", "isLocked"),
    IsRecommendationLockIcon:   Boolean("If the media is locked for recommendations", "media.isRecommendationBlocked", "isRecommendationBlocked"),
    IsReviewBlockedIcon:        Boolean("If the media is review blocked", "media.isReviewBlocked", "isReviewBlocked"),
    CoverImage:                 Image("Cover image of the media", "media.coverImage", "coverImage { extraLarge }"),
    BannerImage:                Image("Banner image of the media", "media.bannerImage", "bannerImage")
}