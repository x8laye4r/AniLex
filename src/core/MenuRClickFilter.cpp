#include "anilex/core/MenuRClickFilter.h"
#include <QObject>
#include <QMouseEvent>

bool MenuRClickFilter::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::MouseButtonRelease) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if (mouseEvent->button() == Qt::RightButton) {
      return true;
    }
  }
  return QObject::eventFilter(obj, event);
}
