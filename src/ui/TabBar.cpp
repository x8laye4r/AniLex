#include "anilex/ui/TabBar.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

TabBar::TabBar(const QList<Tab> &tabs, QWidget *parent)
    : QWidget(parent) {

    container = new QFrame(this);
    container->setObjectName("navbarContainer");

    layout = new QHBoxLayout(container);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->setSpacing(5);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    for (const auto &tab : tabs) {
        addTab(tab);
    }

    QHBoxLayout *baseLayout = new QHBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->addWidget(container);

    connect(buttonGroup, &QButtonGroup::idClicked, this, &TabBar::tabChanged);
}

void TabBar::addTab(const Tab &tab) {
    QPushButton *btn = new QPushButton(tab.name);
    btn->setCheckable(true);

    int id = buttonGroup->buttons().size();
    if (id == 0) btn->setChecked(true);

    buttonGroup->addButton(btn, id);
    layout->addWidget(btn);

    if (!tabs.contains(tab)) {
        tabs.append(tab);
    }
}