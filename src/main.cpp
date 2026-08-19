#include <any>
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QFile>
#include <QDebug>
#include <QWidget>
#include <dlfcn.h>

#include "generated/operations/RateReviewMutation.hpp"
#include "ui/MainWindow.h"

static void anilexLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
  QString levelText;

  switch (type) {
    case QtDebugMsg: levelText = "DEBUG"; break;
    case QtInfoMsg: levelText = "INFO"; break;
    case QtWarningMsg: levelText = "WARNING"; break;
    case QtCriticalMsg: levelText = "CRITICAL"; break;
    case QtFatalMsg: levelText = "FATAL"; break;
  }

  QString file = context.file ? QString(context.file) : "Unknown";
  QString function = context.function ? QString(context.function) : "Unknown";

  const QString logLine = QString("[%1] [%2] %3 (%4:%5, %6)\n")
                                  .arg(timeStr, levelText, msg, file,
                                    QString::number(context.line), function);

  QFile logFile("anilex.log");
  if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
    QTextStream out(&logFile);
    out << logLine;
  }

  fprintf(stderr, "%s", logLine.toLocal8Bit().constData());
  fflush(stderr);

  if (type == QtFatalMsg) {
    abort();
  }
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QApplication::setApplicationName("AniLex");
  QApplication::setApplicationVersion("0.0.1");

  qInstallMessageHandler(anilexLogHandler);

  QTranslator translator;
  if (translator.load(QLocale::system(), "anilex", "_", ":/i18n")) {
    qInfo() << "Loaded translation:" << translator.filePath();
    QApplication::installTranslator(&translator);
  } else {
    qWarning() << "Failed to load translation" << translator.filePath();
  }

  MainWindow window;
  window.show();

  return QApplication::exec();
}