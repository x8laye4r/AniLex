#include "anilex/ui/TabBar.h"

TabBar::TabBar(const QList<Tab> &tabs, QWidget *parent) : QWidget(parent), tabs(tabs) {
    this->layout = new QVBoxLayout(this);
    this->tabBar = new QFrame(this);
    this->tabBar->setObjectName("tabBar");

    this->setupUI();
}

void TabBar::setupUI() {
    this->tabBar->setFrameStyle(QFrame::StyledPanel);

    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->addWidget(this->tabBar);

}

void TabBar::setupConnections() {
}



