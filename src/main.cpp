#include <QApplication>
#include <qexception.h>
#include <anilex/ui/MainWindow.h>
#include <version.h>
#include <QString>
#include <QMessageBox>

#include "anilex/core/AniListEnums.h"
#include "anilex/core/SecretStorage.h"
// #include "AniListApi.h"
#include <QFile>

#include "anilex/core/Authenticator.h"
#include "anilex/core/AuthServer.h"
#include "anilex/core/GlobalSettings.h"

template<typename T>
QString enumToString(const T value) {
    try {
        return QVariant::fromValue(value).toString();
    } catch (QException) {
        return "";
    }
}


void loadStylesheet(QApplication &app) {
    QFile file(":/styles/main.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        app.setStyleSheet(in.readAll());
    }
}

/*
 * TODO: Redo all icons to make them look better
 */

int main(int argc, char *argv[]) {
    QApplication app_anilex(argc, argv);

    GlobalSettings::instance().setValue("Network/Port", 8080);

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

    QList<TabMeta> tabs;
    tabs.append((TabMeta){.name = "HOME", .icon = QString(":/icons/Home.svg")});
    tabs.append((TabMeta){.name = "LIBRARY", .icon = QString(":/icons/Lists.svg")});
    tabs.append((TabMeta){.name = "BROWSE", .icon = QString(":/icons/Browse.svg")});
    tabs.append((TabMeta){.name = "SEARCH", .icon = QString(":/icons/Search.svg")});
    tabs.append((TabMeta){.name = "PROFILE", .icon = QString(":/icons/Profile.svg")});

    MainWindow widget(tabs);

    AuthServer server;
    const Authenticator authenticator;


    QObject::connect(&server, &AuthServer::tokenReceived, &authenticator, &Authenticator::saveApiToken);
    QObject::connect(&authenticator, &Authenticator::finishedAuth, &server, &AuthServer::onAuthResult);
    QObject::connect(&authenticator, &Authenticator::finishedAuth, &server, [&widget](AuthErrors::Errors success) {
        if (success == AuthErrors::Errors::NoError) {
            QMessageBox::information(&widget, QObject::tr("Success"), QObject::tr("Token saved! You can now browse your list!"));
        } else {
            QMessageBox::warning(&widget, QObject::tr("Error"), AuthErrors::convertToErrorString(success));
        }
    });
    Authenticator::startAuth();

    widget.show();

    return QApplication::exec();
}