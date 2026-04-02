#include "anilex/ui/MainWindow.h"

#define MINIMUM_HEIGHT 400
#define MINIMUM_WIDTH 600
#define ANIMATION_DURATION 500

MainWindow::MainWindow(const QList<Tab> &tabs, QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    this->layout = new QVBoxLayout(centralWidget);

    this->tabBar = new TabBar(tabs, ANIMATION_DURATION, this);
    this->stackedLayout = new QStackedLayout();

    this->setupUI(tabs);
    this->setMinimumSize(QSize(MINIMUM_WIDTH, MINIMUM_HEIGHT));
    this->setupConnections();
}

void MainWindow::setupUI(const QList<Tab> &tabs) {
    this->layout->setContentsMargins(0, 0, 0, 0);
    this->layout->setSpacing(0);

    this->layout->addWidget(this->tabBar);

    this->layout->addLayout(this->stackedLayout, 1);

    for (const auto &tab : tabs) {
        QWidget *page = createPage(tab.name);
        this->stackedLayout->addWidget(page);
    }
}

void MainWindow::setupConnections() {
    connect(this->tabBar, &TabBar::tabChanged, this->stackedLayout, &QStackedLayout::setCurrentIndex);
}

QWidget* MainWindow::createPage(const QString &text) {
    QWidget *pageWidget = new QWidget();
    QVBoxLayout *pageLayout = new QVBoxLayout(pageWidget);

    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignCenter);

    pageLayout->addWidget(label);
    return pageWidget;
}