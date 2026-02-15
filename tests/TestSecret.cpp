#include <QtTest>
#include <QSignalSpy>
#include <QUuid>

#include "anilex/core/SecretStorage.h"

class TestSecretStore : public QObject {
    Q_OBJECT
private slots:
    void write_read_delete() {
        SecretStorage keyring(this);

        const QString key = QString("test/%1")
            .arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
        const QString value = "hello-token";

        QSignalSpy storedSpy(&keyring, &SecretStorage::secretStored);
        QSignalSpy loadedSpy(&keyring, &SecretStorage::secretLoaded);
        QSignalSpy deletedSpy(&keyring, &SecretStorage::secretDeleted);
        QSignalSpy errorSpy(&keyring, &SecretStorage::errorOccurred);

        keyring.saveSecret(key, value);
        QTRY_VERIFY_WITH_TIMEOUT(storedSpy.count() == 1 || errorSpy.count() == 1, 5000);
        QVERIFY2(errorSpy.count() == 0, "Keychain store failed (no backend / locked keyring?)");

        keyring.getSecret(key);
        QTRY_VERIFY_WITH_TIMEOUT(loadedSpy.count() == 1, 5000);

        auto args = loadedSpy.takeFirst();
        QCOMPARE(args.at(0).toString(), key);
        QCOMPARE(args.at(1).toString(), value);

        keyring.deleteSecret(key);
        QTRY_VERIFY_WITH_TIMEOUT(deletedSpy.count() == 1 || errorSpy.count() == 1, 5000);
        QVERIFY2(errorSpy.count() == 0, "Keychain delete failed");

        errorSpy.clear();
        loadedSpy.clear();

        keyring.getSecret(key);

        QTRY_VERIFY_WITH_TIMEOUT(loadedSpy.count() == 1 || errorSpy.count() == 1, 5000);

        QVERIFY2(errorSpy.count() == 0, "Read should not error on not-found");

        auto args2 = loadedSpy.takeFirst();
        QCOMPARE(args2.at(0).toString(), key);
        QCOMPARE(args2.at(1).toString(), QString{});
    }
};

QTEST_MAIN(TestSecretStore)
#include "TestSecret.moc"