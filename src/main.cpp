#include <QApplication>
#include <qexception.h>
#include <anilex/ui/MainWindow.h>
#include <version.h>
#include <QString>
#include <QTranslator>

#include "anilex/core/AniListEnums.h"
#include "anilex/core/SecretStorage.h"
// #include "AniListApi.h"
#include <QFile>

#include "anilex/core/Authenticator.h"
#include "anilex/core/GlobalSettings.h"
#include "anilex/ui/designer/Designer.h"
#include "anilex/ui/designer/DesignerItemFactory.h"
#include "anilex/ui/designer/item/DesignerTextItem.h"
#include "anilex/utils/Helper.h"

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

/*
 * TODO: Redo all icons to make them look better
 */

int main(int argc, char *argv[]) {
    QApplication app_anilex(argc, argv);

    // Application Settings
    QApplication::setApplicationName("AniLex");
    QApplication::setApplicationDisplayName("AniLex - AniList Tracker");
    QApplication::setApplicationVersion(VERSION_STR);

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
    /*
    QList<TabMeta> tabs;
    tabs.append((TabMeta){.name = QObject::tr("HOME"), .icon = QString(":/assets/icons/Home.svg")});
    tabs.append((TabMeta){.name = QObject::tr("LIBRARY"), .icon = QString(":/assets/icons/Lists.svg")});
    tabs.append((TabMeta){.name = QObject::tr("BROWSE"), .icon = QString(":/assets/icons/Browse.svg")});
    tabs.append((TabMeta){.name = QObject::tr("SEARCH"), .icon = QString(":/assets/icons/Search.svg")});
    tabs.append((TabMeta){.name = QObject::tr("PROFILE"), .icon = QString(":/assets/icons/Profile.svg")});

    MainWindow widget(tabs);
    widget.show();
    */

    const QString locale = QLocale::system().name();

    QTranslator *translator = new QTranslator(&app_anilex);
    if (translator->load(locale, "anilex" , "_", ":/i18n")) {
        qInfo() << "Loaded translation for locale:" << locale;
        QApplication::installTranslator(translator);
    } else {
        qCritical() << "Failed to load translation for locale:" << locale;
    }

    loadStylesheet(app_anilex);

    Designer designer;
    DesignerItemFactory::instance().printAll();
    designer.show();

    return QApplication::exec();
}