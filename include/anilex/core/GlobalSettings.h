#pragma once
#include <QObject>
#include <QMutexLocker>
#include <QSettings>

class GlobalSettings : public QObject {
    Q_OBJECT
public:
    static GlobalSettings &instance() {
        static GlobalSettings instance;
        return instance;
    }

    QSettings& settings() { return settings_; }

    void setValue(const QString &key, const QVariant &value) {
        QMutexLocker locker(&mutex);
        settings_.setValue(key, value);
    }

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) {
        QMutexLocker locker(&mutex);
        return settings_.value(key, defaultValue);
    }

private:
    GlobalSettings() : settings_(QSettings::IniFormat, QSettings::UserScope, "AniLex", "AniLex") {}
    GlobalSettings(const GlobalSettings&) = delete;
    GlobalSettings& operator=(const GlobalSettings&) = delete;

    QMutex mutex;
    QSettings settings_;
};
