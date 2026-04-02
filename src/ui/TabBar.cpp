#include "anilex/ui/TabBar.h"

namespace TabBarConf {
    struct TabBar {
        static constexpr uint maxHeight = 160;
    };
}

TabBar::TabBar(const QList<Tab> &tabs, const uint animation_duration, QWidget *parent)
    : QWidget(parent), animation_duration(animation_duration) {

    setupUi();
    setupConnections();
    addTabs(tabs);

    if (!tabButtons.isEmpty()) {
        tabButtons.first()->moveDownImmediately();
        tabButtons.first()->setChecked(true);
    }
}

void TabBar::addTabs(const QList<Tab> &tabs) {
    for (int i = 0; i < tabs.size(); ++i) {
        addTab(tabs.at(i), i);
    }
}

void TabBar::setupUi() {
    QHBoxLayout *baseLayout = new QHBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    container = new QFrame(this);
    container->setObjectName("navbarContainer");
    baseLayout->addWidget(container);

    layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    this->setMaximumHeight(TabBarConf::TabBar::maxHeight);
}

void TabBar::setupConnections() {
    connect(buttonGroup, &QButtonGroup::idClicked, this, &TabBar::tabChanged);
    connect(buttonGroup, &QButtonGroup::buttonToggled, this, &TabBar::onButtonToggled);
}

void TabBar::onButtonToggled(QAbstractButton *btn, bool checked) {
    TabButton *tabBtn = qobject_cast<TabButton *>(btn);
    if (tabBtn) {
        checked ? tabBtn->moveDown() : tabBtn->reset();
    }
}

void TabBar::addTab(const Tab &tab, int index) {
    TabButton *btn = new TabButton(tab.name, tab.icon, animation_duration, this);

    layout->addWidget(btn);
    tabButtons.append(btn);
    buttonGroup->addButton(btn, index);
}