#pragma once
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGraphicsScene>

class DesignerWidgetsList : public QScrollArea {
  Q_OBJECT
public:
  explicit DesignerWidgetsList(QWidget* parent = nullptr);

  void addWidgetSections(QMap<QString, QList<QJsonObject> > &widgetsList);

private:
  QWidget *m_centralWidget = nullptr;
  QVBoxLayout *m_centralWidgetLayout = nullptr;
};