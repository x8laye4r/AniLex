#pragma once
#include <QObject>
#include <QJsonObject>

namespace anilex_designer_helpers {
  Q_NAMESPACE

  #define TRANSLATE(x) QCoreApplication::translate("Designer", (x))

  enum class DesignerHelpers {
    NONE,
    TEXT,
    DATE_TEXT,
    SCROLLABLE,
    MEDIA_FLAGS,
    IMAGE
  }; Q_ENUM_NS(DesignerHelpers)

  struct DesignerItem {
    QString displayName;
    QString type;
    QString toolTip;
    QString graphqlSource;
    QString graphqlQuery;
  };

  struct DesignerCategory {
    QString categoryTitle;
    QList<DesignerItem> items;
  };

  static QJsonObject designerItemToJsonObject(const DesignerItem *item) {
    QJsonObject obj;

    obj.insert("displayName", item->displayName);
    obj.insert("type", item->type);
    obj.insert("toolTip", item->toolTip);
    obj.insert("graphqlSource", item->graphqlSource);
    obj.insert("graphqlQuery", item->graphqlQuery);

    return obj;
  }

  static DesignerItem designerItemFromJsonObject(const QJsonObject &obj) {
    DesignerItem item;

    item.displayName = obj.value("displayName").toString();
    item.type = obj.value("type").toString();
    item.toolTip = obj.value("desc").toString();
    item.graphqlSource = obj.value("dataSource").toString();
    item.graphqlQuery = obj.value("graphqlQuery").toString();

    return item;
  }


  static const char* designerItemNames[] = {
        // Category Names
        QT_TRANSLATE_NOOP("Designer", "Labels")

        // Dates
        QT_TRANSLATE_NOOP("Designer", "Completed At"),
        QT_TRANSLATE_NOOP("Designer", "Created At"),
        QT_TRANSLATE_NOOP("Designer", "Started At"),
        QT_TRANSLATE_NOOP("Designer", "Updated At"),
        QT_TRANSLATE_NOOP("Designer", "End Date"),
        QT_TRANSLATE_NOOP("Designer", "Start Date"),

        // Text Labels
        QT_TRANSLATE_NOOP("Designer", "Global ID"),
        QT_TRANSLATE_NOOP("Designer", "Media ID"),
        QT_TRANSLATE_NOOP("Designer", "Notes"),
        QT_TRANSLATE_NOOP("Designer", "Priority"),
        QT_TRANSLATE_NOOP("Designer", "Repeats"),
        QT_TRANSLATE_NOOP("Designer", "Score"),
        QT_TRANSLATE_NOOP("Designer", "Status"),
        QT_TRANSLATE_NOOP("Designer", "Average Score"),
        QT_TRANSLATE_NOOP("Designer", "Origin"),
        QT_TRANSLATE_NOOP("Designer", "Description"),
        QT_TRANSLATE_NOOP("Designer", "Duration"),
        QT_TRANSLATE_NOOP("Designer", "Favorites"),
        QT_TRANSLATE_NOOP("Designer", "Format"),
        QT_TRANSLATE_NOOP("Designer", "Hashtag"),
        QT_TRANSLATE_NOOP("Designer", "Mean Score"),
        QT_TRANSLATE_NOOP("Designer", "Popularity"),
        QT_TRANSLATE_NOOP("Designer", "Season"),
        QT_TRANSLATE_NOOP("Designer", "Season Year"),
        QT_TRANSLATE_NOOP("Designer", "Source"),
        QT_TRANSLATE_NOOP("Designer", "Title"),
        QT_TRANSLATE_NOOP("Designer", "Trending"),
        QT_TRANSLATE_NOOP("Designer", "Type"),
        QT_TRANSLATE_NOOP("Designer", "Next Episode"),
        QT_TRANSLATE_NOOP("Designer", "Trailer"),

        // Scrollareas
        QT_TRANSLATE_NOOP("Designer", "Advanced Scores"),
        QT_TRANSLATE_NOOP("Designer", "Custom Lists"),
        QT_TRANSLATE_NOOP("Designer", "Airing Schedule"),
        QT_TRANSLATE_NOOP("Designer", "External Links"),
        QT_TRANSLATE_NOOP("Designer", "Genres"),
        QT_TRANSLATE_NOOP("Designer", "Rankings"),
        QT_TRANSLATE_NOOP("Designer", "Relations"),
        QT_TRANSLATE_NOOP("Designer", "Staff"),
        QT_TRANSLATE_NOOP("Designer", "Score Distribution"),
        QT_TRANSLATE_NOOP("Designer", "Status Distribution"),
        QT_TRANSLATE_NOOP("Designer", "Studios"),
        QT_TRANSLATE_NOOP("Designer", "Synonyms"),
        QT_TRANSLATE_NOOP("Designer", "Tags"),

        // Booleans / Icons
        QT_TRANSLATE_NOOP("Designer", "Hidden"),
        QT_TRANSLATE_NOOP("Designer", "Private"),
        QT_TRANSLATE_NOOP("Designer", "Adult"),
        QT_TRANSLATE_NOOP("Designer", "Favorite"),
        QT_TRANSLATE_NOOP("Designer", "Licensed"),
        QT_TRANSLATE_NOOP("Designer", "Locked"),
        QT_TRANSLATE_NOOP("Designer", "Recommendation Blocked"),
        QT_TRANSLATE_NOOP("Designer", "Review Blocked"),

        // Images
        QT_TRANSLATE_NOOP("Designer", "Cover Image"),
        QT_TRANSLATE_NOOP("Designer", "Banner Image")
    };

} // anilex_designer_helpers namespace