#pragma once
#include <qmenu.h>
#include <QObject>

class MenuRClickFilter : public QObject {
  Q_OBJECT
public:
  explicit MenuRClickFilter(QObject *parent = nullptr) : QObject(parent) {};
protected:
  bool eventFilter(QObject *obj, QEvent *event) override;
};
