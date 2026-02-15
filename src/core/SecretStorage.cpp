#include "anilex/core/SecretStorage.h"
#include <qtkeychain/keychain.h>


SecretStorage::SecretStorage(QObject *parent) : QObject(parent) {}

void SecretStorage::saveSecret(const QString &key, const QString &value) {
    auto *job = new QKeychain::WritePasswordJob(service, this);
    job->setKey(key);
    job->setTextData(value);

    connect(job, &QKeychain::Job::finished, this, [this, job, key]() {
       if (job->error()) emit errorOccurred(key, job->errorString());
        else emit secretStored(key);
        job->deleteLater();
    });

    job->start();
}

void SecretStorage::getSecret(const QString &key) {
    auto *job = new QKeychain::ReadPasswordJob(service, this);
    job->setKey(key);

    connect(job, &QKeychain::Job::finished, this, [this, job, key]() {
        if (job->error()) {
            emit secretLoaded(key, QString{});
        } else {
            emit secretLoaded(key, job->textData());
        }
        job->deleteLater();
    });
    job->start();
}

void SecretStorage::deleteSecret(const QString &key) {
    auto *job = new QKeychain::DeletePasswordJob(service, this);
    job->setKey(key);

    connect(job, &QKeychain::Job::finished, this, [this, job, key]() {
        if (job->error()) emit errorOccurred(key, job->errorString());
        else emit secretDeleted(key);
        job->deleteLater();
    });
    job->start();
}
