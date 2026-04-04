#include "anilex/ui/TabBarSimple.h"

#include "anilex/ui/TabBar.h"
#include "anilex/ui/TabButtonSimple.h"

namespace TabBarConf {
    struct TabBar {
        static constexpr uint maxHeight = 160;
        static constexpr uint margin = 5;
    };
}

TabBarSimple::TabBarSimple(const QList<TabMeta> &tabs, QWidget *parent)
    : AbstractTabBar(parent) {
    TabBarSimple::setupUi();
    addTabs(tabs);

    if (!tabButtons.isEmpty()) {
        tabButtons.first()->animationInstant();
        tabButtons.first()->setChecked(true);
    }

    TabBarSimple::setupConnections();

}

void TabBarSimple::setupUi() {
    QHBoxLayout *baseLayout = new QHBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    container = new QFrame(this);
    container->setObjectName("navbarContainer");
    baseLayout->addWidget(container);

    layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, TabBarConf::TabBar::margin);
    layout->setSpacing(0);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    indicator = new QWidget(container);
    indicator->setObjectName("indicator");
    indicator->setFixedHeight(3);
    indicator->raise();


    this->setMaximumHeight(TabBarConf::TabBar::maxHeight);
}

void TabBarSimple::setupConnections() {
    connect(buttonGroup, &QButtonGroup::idClicked, this, &TabBarSimple::tabChanged);
    connect(buttonGroup, &QButtonGroup::buttonToggled, this, &TabBarSimple::onButtonClicked);
}

void TabBarSimple::onButtonClicked(QAbstractButton *button, bool checked) {
    TabButtonSimple *btn = qobject_cast<TabButtonSimple*>(button);
    if (btn) {
        if (btn->isChecked()) {
            btn->startAnimation();
            updateIndicator(btn, false);
            indicator->raise();
        } else {
            btn->endAnimation();
        }
    }
}

void TabBarSimple::addTab(const TabMeta &tab, int index) {
    TabButtonSimple *btn = new TabButtonSimple(tab.name, this);
    layout->addWidget(btn);
    tabButtons.append(btn);
    buttonGroup->addButton(btn, index);
}

void TabBarSimple::addTabs(const QList<TabMeta> &tabs) {
    for (int i = 0; i < tabs.size(); ++i) {
        addTab(tabs.at(i), i);
    }
}

void TabBarSimple::resizeEvent(QResizeEvent *event) {
    AbstractTabBar::resizeEvent(event);
    if (auto *activeBtn = qobject_cast<TabButtonSimple*>(buttonGroup->checkedButton())) {
        updateIndicator(activeBtn, true);
    }
}

void TabBarSimple::updateIndicator(TabButtonSimple* target, bool immediate) {
    if (!target) return;

    const int y = target->geometry().bottom() + indicator->height() + (TabBarConf::TabBar::margin - indicator->height());
    QRect targetRect(target->x(), y, target->width(), indicator->height());

    if (immediate) {
        indicator->setGeometry(targetRect);
    } else {
        QPropertyAnimation *anim = new QPropertyAnimation(indicator, "geometry");
        anim->setDuration(GlobalSettings::instance().value("Animation/duration", 500).toInt());
        anim->setStartValue(indicator->geometry());
        anim->setEndValue(targetRect);
        anim->setEasingCurve(QEasingCurve::OutQuint);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
