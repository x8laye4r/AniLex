#pragma once
#include <QString>
#include <QObject>

class AuthErrors : public QObject {
  Q_OBJECT
public:
  enum class Errors {
    NoError,
    ServerStartFail,
    KeyringSaveError,
    UserFetchError

  }; Q_ENUM(Errors);

  static QString convertToErrorString(Errors error) {
    switch (error) {
      case Errors::NoError:
        return tr("No error accoured");
      case Errors::ServerStartFail:
        return tr("Failed to start the server");
      case Errors::KeyringSaveError:
        return tr("Failed to save token to the keyring");
      case Errors::UserFetchError:
        return tr("Failed to fetch user");
      default:
        return tr("Unknown error");
    }
  }

};
