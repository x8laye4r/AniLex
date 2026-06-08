#include "anilex/ui/designer/DesignerWidgetsList.h"

#include <qcoreapplication.h>
#include <QJsonObject>

#include "anilex/ui/designer/DesignerWidgetsListSection.h"

DesignerWidgetsList::DesignerWidgetsList(QWidget *parent)
  : QScrollArea(parent) {
  m_centralWidget = new QWidget(this);
  m_centralWidgetLayout = new QVBoxLayout(m_centralWidget);

  m_centralWidgetLayout->setAlignment(Qt::AlignTop);

  m_centralWidget->setLayout(m_centralWidgetLayout);

  this->setWidget(m_centralWidget);
  this->setWidgetResizable(true);
}

void DesignerWidgetsList::addWidgetSections(QMap<QString, QList<QJsonObject>> &widgetsList) {
  for (auto item = widgetsList.begin(); item != widgetsList.end(); ++item) {
    QString name = QCoreApplication::translate("DesignerPresets", item.key().toUtf8().constData());
    QList<QJsonObject> obj = item.value();

    DesignerWidgetsListSection *section = new DesignerWidgetsListSection(name, obj, this);

    m_centralWidgetLayout->addWidget(section);
  }
}

void dummy_json_translations() {
    QT_TRANSLATE_NOOP("DesignerPresets", "Adult");
    QT_TRANSLATE_NOOP("DesignerPresets", "Advanced Scores");
    QT_TRANSLATE_NOOP("DesignerPresets", "Airing Schedule");
    QT_TRANSLATE_NOOP("DesignerPresets", "Average Score");
    QT_TRANSLATE_NOOP("DesignerPresets", "Banner Image");
    QT_TRANSLATE_NOOP("DesignerPresets", "Completed At");
    QT_TRANSLATE_NOOP("DesignerPresets", "Cover Image");
    QT_TRANSLATE_NOOP("DesignerPresets", "Created At");
    QT_TRANSLATE_NOOP("DesignerPresets", "Custom Lists");
    QT_TRANSLATE_NOOP("DesignerPresets", "Description");
    QT_TRANSLATE_NOOP("DesignerPresets", "Duration");
    QT_TRANSLATE_NOOP("DesignerPresets", "End Date");
    QT_TRANSLATE_NOOP("DesignerPresets", "External Links");
    QT_TRANSLATE_NOOP("DesignerPresets", "Favorite");
    QT_TRANSLATE_NOOP("DesignerPresets", "Favorites");
    QT_TRANSLATE_NOOP("DesignerPresets", "Format");
    QT_TRANSLATE_NOOP("DesignerPresets", "Genres");
    QT_TRANSLATE_NOOP("DesignerPresets", "Global ID");
    QT_TRANSLATE_NOOP("DesignerPresets", "Hashtag");
    QT_TRANSLATE_NOOP("DesignerPresets", "Hidden");
    QT_TRANSLATE_NOOP("DesignerPresets", "Licensed");
    QT_TRANSLATE_NOOP("DesignerPresets", "Locked");
    QT_TRANSLATE_NOOP("DesignerPresets", "Mean Score");
    QT_TRANSLATE_NOOP("DesignerPresets", "Media ID");
    QT_TRANSLATE_NOOP("DesignerPresets", "Next Episode");
    QT_TRANSLATE_NOOP("DesignerPresets", "Notes");
    QT_TRANSLATE_NOOP("DesignerPresets", "Origin");
    QT_TRANSLATE_NOOP("DesignerPresets", "Popularity");
    QT_TRANSLATE_NOOP("DesignerPresets", "Priority");
    QT_TRANSLATE_NOOP("DesignerPresets", "Private");
    QT_TRANSLATE_NOOP("DesignerPresets", "Rankings");
    QT_TRANSLATE_NOOP("DesignerPresets", "Recommendation Blocked");
    QT_TRANSLATE_NOOP("DesignerPresets", "Relations");
    QT_TRANSLATE_NOOP("DesignerPresets", "Repeats");
    QT_TRANSLATE_NOOP("DesignerPresets", "Review Blocked");
    QT_TRANSLATE_NOOP("DesignerPresets", "Score");
    QT_TRANSLATE_NOOP("DesignerPresets", "Score Distribution");
    QT_TRANSLATE_NOOP("DesignerPresets", "Season");
    QT_TRANSLATE_NOOP("DesignerPresets", "Season Year");
    QT_TRANSLATE_NOOP("DesignerPresets", "Source");
    QT_TRANSLATE_NOOP("DesignerPresets", "Staff");
    QT_TRANSLATE_NOOP("DesignerPresets", "Start Date");
    QT_TRANSLATE_NOOP("DesignerPresets", "Started At");
    QT_TRANSLATE_NOOP("DesignerPresets", "Status");
    QT_TRANSLATE_NOOP("DesignerPresets", "Status Distribution");
    QT_TRANSLATE_NOOP("DesignerPresets", "Studios");
    QT_TRANSLATE_NOOP("DesignerPresets", "Synonyms");
    QT_TRANSLATE_NOOP("DesignerPresets", "Tags");
    QT_TRANSLATE_NOOP("DesignerPresets", "Title");
    QT_TRANSLATE_NOOP("DesignerPresets", "Trailer");
    QT_TRANSLATE_NOOP("DesignerPresets", "Trending");
    QT_TRANSLATE_NOOP("DesignerPresets", "Type");
    QT_TRANSLATE_NOOP("DesignerPresets", "Updated At");

    QT_TRANSLATE_NOOP("DesignerPresets", "AdvancedScoreScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "AiringScheduleScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "AverageScoreLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "BannerImage");
    QT_TRANSLATE_NOOP("DesignerPresets", "CompletedAtLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "CountryOfOriginLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "CoverImage");
    QT_TRANSLATE_NOOP("DesignerPresets", "CreatedAtLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "CustomListsScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "DescriptionLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "DurationLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "EndDateLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "ExternalLinksScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "FavouritesLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "FormatHashtag");
    QT_TRANSLATE_NOOP("DesignerPresets", "FormatLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "GenresScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "HiddenFromStatusListIcon");
    QT_TRANSLATE_NOOP("DesignerPresets", "IdLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "IsAdultIcon");
    QT_TRANSLATE_NOOP("DesignerPresets", "IsFavoriteIcon");
    QT_TRANSLATE_NOOP("DesignerPresets", "IsLicensedIcon");
    QT_TRANSLATE_NOOP("DesignerPresets", "IsLockedIcon");
    QT_TRANSLATE_NOOP("DesignerPresets", "IsRecommendationLockIcon");
    QT_TRANSLATE_NOOP("DesignerPresets", "IsReviewBlockedIcon");
    QT_TRANSLATE_NOOP("DesignerPresets", "MeanscoreLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "MediaIdLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "NextAiringEpisodeLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "NotesLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "PopularityLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "PriorityLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "PrivateIcon");
    QT_TRANSLATE_NOOP("DesignerPresets", "RankingsScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "RelationsScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "RepeatLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "ScoreDistributionScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "ScoreLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "SeasonLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "SeasonYearLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "SourceLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "StaffScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "StartDateLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "StartedAtLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "StatusDistributionScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "StatusLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "StudiosScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "SynonymsScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "TagsScrollarea");
    QT_TRANSLATE_NOOP("DesignerPresets", "TitleLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "TrailerLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "TrendingLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "TypeLabel");
    QT_TRANSLATE_NOOP("DesignerPresets", "UpdatedAtLabel");
}