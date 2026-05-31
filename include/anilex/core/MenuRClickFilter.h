#pragma once
#include <QObject>

class MenuRClickFilter : public QObject {
protected:
  bool eventFilter(QObject *obj, QEvent *event) override;
};
