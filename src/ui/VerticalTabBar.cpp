#include "anilex/ui/VerticalTabBar.h"

#include <QPainter>

#include "anilex/ui/TabBar.h"
#include "anilex/ui/TabButtonSimple.h"

namespace TabBarConf {
    struct TabBar {
        static constexpr double width = 0.3f;

        static constexpr uint maxHeight = 160;
        static constexpr uint maxWidth = 300;
        static constexpr uint margin = 5;
    };
}

VerticalTabBar::VerticalTabBar(const QList<TabMeta> &tabs, QWidget *parent)
    : AbstractTabBar(parent) {
    VerticalTabBar::setupUi();
    addTabs(tabs);

    if (!tabButtons.isEmpty()) {
        tabButtons.first()->animationInstant();
        tabButtons.first()->setChecked(true);
    }

    VerticalTabBar::setupConnections();

}

void VerticalTabBar::setupUi() {
    QVBoxLayout *baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    container = new QFrame(this);
    container->setObjectName("navbarContainer");
    baseLayout->addWidget(container);

    layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);
    layout->setAlignment(Qt::AlignTop);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    indicator = new QWidget(container);
    indicator->setObjectName("indicator");
    indicator->setFixedWidth(3);
    indicator->raise();

    this->setFixedWidth(qMin(static_cast<uint>(this->window()->width() * TabBarConf::TabBar::width), TabBarConf::TabBar::maxWidth));
    this->setMaximumHeight(16777215);
}

void VerticalTabBar::setupConnections() {
    connect(buttonGroup, &QButtonGroup::idClicked, this, &TabBar::tabChanged);
    connect(buttonGroup, &QButtonGroup::buttonToggled, this, &VerticalTabBar::onButtonClicked);
}

void VerticalTabBar::onButtonClicked(QAbstractButton *button, bool checked) {
    TabButtonSimple *btn = qobject_cast<TabButtonSimple *>(button);
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

void VerticalTabBar::addTab(const TabMeta &tab, int index) {
    TabButtonSimple *btn = new TabButtonSimple(tab.name, this);
    layout->addWidget(btn);
    tabButtons.append(btn);
    buttonGroup->addButton(btn, index);
}

void VerticalTabBar::addTabs(const QList<TabMeta> &tabs) {
    for (int i = 0; i < tabs.size(); i++) {
        addTab(tabs.at(i), i);
    }
}

void VerticalTabBar::updateIndicator(TabButtonSimple *target, bool immediate) {
    if (!target) return;

    QRect targetRect(0, target->y(), indicator->width(), target->height());

    if (immediate) {
        indicator->setGeometry(targetRect);
    } else {
        QPropertyAnimation *animation = new QPropertyAnimation(indicator, "geometry");
        animation->setDuration(GlobalSettings::instance().value("Animation/duration", 500).toInt());
        animation->setStartValue(indicator->geometry());
        animation->setEndValue(targetRect);
        animation->setEasingCurve(QEasingCurve::OutQuint);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void VerticalTabBar::resizeEvent(QResizeEvent *event) {
    AbstractTabBar::resizeEvent(event);
    if (auto *activeBtn = qobject_cast<TabButtonSimple*>(buttonGroup->checkedButton())) {
        updateIndicator(activeBtn, true);
        this->setFixedWidth(qMin(static_cast<uint>(this->window()->width() * TabBarConf::TabBar::width), TabBarConf::TabBar::maxWidth));
    }
}

