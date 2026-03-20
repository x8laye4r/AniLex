#ifndef ANILEX_TABBAR_H
#define ANILEX_TABBAR_H
#include <QWidget>
#include <QFrame>
#include <QList>
#include <QString>
#include <QStackedLayout>
#include <QButtonGroup>

#include "Collapsable.h"
#include "anilex/core/Tabs.h"

class TabBar : public QWidget {
    Q_OBJECT
private:
    QList<Tab> tabs;
    QFrame *container = nullptr;
    QHBoxLayout *layout = nullptr;
    QButtonGroup *buttonGroup = nullptr;

    void setupUI();
    void setupConnections();

public:
    explicit TabBar(const QList<Tab> &tabs, QWidget *parent = nullptr);
    void addTab(const Tab &tab);

signals:
    void tabChanged(int index);
};

#endif //ANILEX_TABBAR_H