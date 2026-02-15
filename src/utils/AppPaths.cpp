#include "anilex/utils/AppPaths.h"


QString AppPaths::cachePath() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir().mkdir(path);
    return path;
}

QString AppPaths::appDataPath() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkdir(path);
    return path;
}


