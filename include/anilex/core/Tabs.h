#ifndef ANILEX_TABS_H
#define ANILEX_TABS_H
#include <QString>

typedef struct Tab {
    QString name;
    QString icon;

    bool operator==(const Tab &other) const {
        return name == other.name and icon == other.icon;
    }

}Tab;

#endif //ANILEX_TABS_H