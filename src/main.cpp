#include <QApplication>
#include <qexception.h>
#include <anilex/ui/MainWindow.h>
#include <../include/anilex/ui/Collapsable.h>
#include <version.h>
#include <QString>

#include "anilex/core/AniListEnums.h"
#include "anilex/core/SecretStorage.h"
#include "AniListApi.h"
#include "anilex/core/Authenticator.h"

template<typename T>
QString enumToString(const T value) {
    try {
        return QVariant::fromValue(value).toString();
    } catch (QException) {
        return "";
    }
}


int main(int argc, char *argv[]) {
    QApplication app_anilex(argc, argv);

    // Application Settings
    app_anilex.setApplicationName("AniLex");
    app_anilex.setApplicationDisplayName("AniLex - AniList Tracker");
    app_anilex.setApplicationVersion(VERSION_STR);

    /*
    SecretStorage keyring;
    AniListApi api;
    QObject::connect(&keyring, &SecretStorage::secretLoaded, [&api](const QString &key, const QString &value) {
        api.doRequest(value, "NewList");
    });
    // keyring.getSecret("auth");
    */

    Authenticator auth;

    MainWindow widget;

    widget.show();

    return app_anilex.exec();
}