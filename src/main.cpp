#include <QApplication>
#include <anilex/ui/MainWindow.h>
#include <anilex/core/Collapsable.h>
#include <version.h>
#include <QString>
#include "anilex/core/AniListEnums.h"

int main(int argc, char *argv[]) {
    QApplication app_anilex(argc, argv);

    // Application Settings
    app_anilex.setApplicationName("AniLex");
    app_anilex.setApplicationDisplayName("AniLex - AniList Tracker");
    app_anilex.setApplicationVersion(VERSION_STR);



    MainWindow widget;

    widget.show();

    return app_anilex.exec();
}