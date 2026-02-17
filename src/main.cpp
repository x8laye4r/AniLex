#include <iostream>
#include <QApplication>
#include <anilex/ui/MainWindow.h>
#include <version.h>
#include <QtGlobal>
#include <QDebug>
#include <QDir>
#include <qdiriterator.h>
#include <QFile>
#include <QString>

int main(int argc, char *argv[]) {
    QApplication app_anilex(argc, argv);

    // Application Settings
    app_anilex.setApplicationName("AniLex");
    app_anilex.setApplicationDisplayName("AniLex - AniList Tracker");
    app_anilex.setApplicationVersion(VERSION_STR);

    // qInfo() << app_anilex.applicationVersion();
    // qInfo() << QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    qInfo() << QT_VERSION_STR;

    QFile file(":/graphql/activity/ToggleActivitySubcriptionMutation.graphql");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << file.errorString();
        return 0;
    }
    QString input = file.readAll();
    qInfo().noquote() << input;


    MainWindow widget;
    widget.show();

    return app_anilex.exec();
}