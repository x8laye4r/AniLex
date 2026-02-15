#pragma once
#include <QObject>

class SecretStorage : public QObject {
    Q_OBJECT
public:
    explicit SecretStorage(QObject *parent = nullptr);

    void saveSecret(const QString &key, const QString &value);
    void getSecret(const QString &key);
    void deleteSecret(const QString &key);

signals:
    void secretLoaded(const QString &key, const QString &value);
    void secretStored(const QString &key);
    void secretDeleted(const QString key);
    void errorOccurred(const QString &key, const QString &message);

private:
    static constexpr auto service = "AniLex";
};

