//
// Created by daniel on 2/15/26.
//

#ifndef ANILEX_APPPATHS_H
#define ANILEX_APPPATHS_H

#include <QStandardPaths>
#include <QString>
#include <QDir>

class AppPaths {
public:
    static QString cachePath();
    static QString appDataPath();
};


#endif //ANILEX_APPPATHS_H