#include "anilex/ui/TabBar.h"

#include <QPainter>
#include <QTimer>
#include "anilex/ui/TabButtonSimple.h"
#include <QStackedLayout>

namespace TabBarConf {
    struct TabBar {
        static constexpr uint maxHeight = 160;
    };
}

TabBar::TabBar(const QList<TabMeta> &tabs, QWidget *parent)
    : AbstractTabBar(parent){
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(GlobalSettings::instance().value("Animation/duration", 500).toInt());

    TabBar::setupUi();
    TabBar::setupConnections();

    addTabs(tabs);

    if (!tabButtons.isEmpty()) {
        tabButtons.first()->animationInstant();
        tabButtons.first()->setChecked(true);
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
    connect(timer, &QTimer::timeout, this, [this]() {
        this->tabChanged(pendingIndex);
    });

    connect(buttonGroup, &QButtonGroup::idClicked, this, [this](int index) {
        pendingIndex = index;
        timer->start();
    });

    connect(buttonGroup, &QButtonGroup::buttonToggled, this, &TabBar::onButtonClicked);
}


void TabBar::onButtonClicked(QAbstractButton *btn, bool checked) {
    TabButton *tab = qobject_cast<TabButton *>(btn);

    if (tab) {
        checked ? tab->startAnimation() : tab->endAnimation();
    }
}

void TabBar::addTabs(const QList<TabMeta> &tabs) {
    for (int i = 0; i < tabs.size(); ++i) {
        addTab(tabs.at(i), i);
    }
}

void TabBar::addTab(const TabMeta &tab, int index) {
        TabButton *btn = new TabButton(tab.name, tab.icon, this);
        layout->addWidget(btn);
        tabButtons.append(btn);
        buttonGroup->addButton(btn, index);
}