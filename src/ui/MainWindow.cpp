#include "anilex/ui/MainWindow.h"

#include "anilex/ui/TabBarSimple.h"
#include "anilex/ui/VerticalTabBar.h"

#define MINIMUM_HEIGHT 400
#define MINIMUM_WIDTH 600

// TODO trying to make a nice fade in fade out animation for the QStackedLayout

static AbstractTabBar* chooseTabBar(const QList<TabMeta> &tabs, QWidget *parent = nullptr) {
    QByteArray tab = GlobalSettings::instance().value("Settings/TabBar", "horizontal-complex").toByteArray();
    if (tab == "horizontal-complex") {
        return new TabBar(tabs, parent);
    }
    if (tab == "horizontal") {
        return new TabBarSimple(tabs, parent);
    }
    if (tab == "vertical") {
        return new VerticalTabBar(tabs, parent);
    }
    return new TabBar(tabs, parent);
}

MainWindow::MainWindow(const QList<TabMeta> &tabs, QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    tabBar = chooseTabBar(tabs, this);
    this->stackedLayout = new QStackedLayout();
    this->stackedLayout->setObjectName("stackedLayout");

    this->setupUI(tabs);
    this->setMinimumSize(QSize(MINIMUM_WIDTH, MINIMUM_HEIGHT));
    this->setupConnections();
}

void MainWindow::setupUI(const QList<TabMeta> &tabs) {
    QByteArray direction = GlobalSettings::instance().value("Settings/TabBar", "horizontal-complex").toByteArray();

    if (this->centralWidget()->layout()) {
        delete this->centralWidget()->layout();
    }

    if (direction == "vertical") {
        QHBoxLayout *hLayout = new QHBoxLayout(this->centralWidget());
        hLayout->setContentsMargins(0, 0, 0, 0);
        hLayout->setSpacing(0);
        hLayout->addWidget(this->tabBar);
        hLayout->addLayout(this->stackedLayout, 1);
        this->layout = hLayout;
    } else {
        QVBoxLayout *vLayout = new QVBoxLayout(this->centralWidget());
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->setSpacing(0);
        vLayout->addWidget(this->tabBar);
        vLayout->addLayout(this->stackedLayout, 1);
        this->layout = vLayout;
    }

    for (const auto &tab : tabs) {
        this->stackedLayout->addWidget(createPage(tab.name));
    }
}

void MainWindow::setupConnections() {
    connect(this->tabBar, &AbstractTabBar::tabChanged, this->stackedLayout, &QStackedLayout::setCurrentIndex);
}

QWidget* MainWindow::createPage(const QString &text) {
    QWidget *pageWidget = new QWidget();
    QVBoxLayout *pageLayout = new QVBoxLayout(pageWidget);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignCenter);

    pageLayout->addWidget(label);
    return pageWidget;
}