#ifndef ANILEX_TABBAR_H
#define ANILEX_TABBAR_H
#include <QWidget>
#include <QFrame>
#include <QList>
#include <QString>
#include <QStackedLayout>
#include "anilex/core/Tabs.h"

class TabBar : public QWidget {
    Q_OBJECT
private:
    QFrame *tabBar = nullptr;
    QVBoxLayout *layout = nullptr;
    QList<Tab> tabs;

    void setupUI();
    void setupConnections();

public:
    explicit TabBar(const QList<Tab> &tabs, QWidget *parent = nullptr);

signals:
    void tabSwitched(int index);
};

#endif //ANILEX_TABBAR_H