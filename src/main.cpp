#include <QApplication>
#include <anilex/ui/MainWindow.h>
#include <version.h>

int main(int argc, char *argv[]) {
    QApplication app_anilex(argc, argv);

    // Application Settings
    app_anilex.setApplicationName("AniLex");
    app_anilex.setApplicationDisplayName("AniLex - AniList Tracker");
    app_anilex.setApplicationVersion(VERSION_STR);

    // qInfo() << app_anilex.applicationVersion();
    // qInfo() << QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

    MainWindow widget;
    widget.show();

    return app_anilex.exec();
}
