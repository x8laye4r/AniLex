#pragma once
#include <QObject>
#include <QMutexLocker>
#include <QSettings>

/**
 * @class GlobalSettings
 * @brief Provides a global accessible thread-safe application-wide settings
 *
 * This class acts as a Singleton (similar to QThreadPool::globalInstance()) to manage all the settings the user is going to safe
 * during the use of this application. The configuration uses the INI-Format and makes sure that read/write/delete operations are protected across
 * different threads using the QMutexLocker
 *
 * @note thread-safe for @ref value(), @ref setValue() and @ref setValue()
 */
class GlobalSettings : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Access to the Singleton instance
     * @return reference to the GlobalSettings
     */
    static GlobalSettings &instance() {
        static GlobalSettings instance;
        return instance;
    }

    /**
     * @brief returns the QSettings variable of this class
     * @warning not thread safe to access
     * @return QSettings
     */
    QSettings& settings() { return settings_; }

    /**
     * @brief This function is storing an option
     * @param key access key
     * @param value value which should be stored
     */
    void setValue(const QString &key, const QVariant &value) {
        QMutexLocker locker(&mutex);
        settings_.setValue(key, value);
    }

    /**
     * @brief This function is deleting an option
     * @param key key to get
     */
    void deleteValue(const QString &key) {
        QMutexLocker locker(&mutex);
        settings_.remove(key);
    }

    /**
     * @brief gets a setting. If not available return the @var defaultValue
     * @param key the key to get
     * @param defaultValue the default value to return
     * @return QVariant
     * @note if no default value is given the default value is initialized with an empty QVariant
     */
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
