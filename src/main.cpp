#include <QApplication>
#include <qexception.h>
#include <anilex/ui/MainWindow.h>
#include <../include/anilex/ui/Collapsable.h>
#include <version.h>
#include <QString>

#include "anilex/core/AniListEnums.h"
#include "anilex/core/SecretStorage.h"
// #include "AniListApi.h"
#include <QFile>

#include "anilex/core/Authenticator.h"

template<typename T>
QString enumToString(const T value) {
    try {
        return QVariant::fromValue(value).toString();
    } catch (QException) {
        return "";
    }
}


void loadStylesheet(QApplication &app) {
    QFile file(":/assets/styles/main.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        app.setStyleSheet(in.readAll());
    }
}

int main(int argc, char *argv[]) {
    QApplication app_anilex(argc, argv);

    // Application Settings
    app_anilex.setApplicationName("AniLex");
    app_anilex.setApplicationDisplayName("AniLex - AniList Tracker");
    app_anilex.setApplicationVersion(VERSION_STR);
    // app_anilex.setDesktopFileName("com.example.anilex");

    /*
    SecretStorage keyring;
    AniListApi api;
    QObject::connect(&keyring, &SecretStorage::secretLoaded, [&api](const QString &key, const QString &value) {
        api.doRequest(value, "NewList");
    });
    // keyring.getSecret("auth");
    */

    //Authenticator auth;
    //auth.startAuth();

    loadStylesheet(app_anilex);

    QList<Tab> tabs;
    tabs.append((Tab){.name = "HOME", .icon = QString(":/assets/icons/Home.png")});
    tabs.append((Tab){.name = "LIBRARY", .icon = QString(":/assets/icons/Lists.png")});
    tabs.append((Tab){.name = "BROWSE", .icon = QString(":/assets/icons/Browse.png")});
    tabs.append((Tab){.name = "SEARCH", .icon = QString(":/assets/icons/Search.png")});
    tabs.append((Tab){.name = "PROFILE", .icon = QString(":/assets/icons/Profile.png")});

    MainWindow widget(tabs);

    widget.show();

    return app_anilex.exec();
}