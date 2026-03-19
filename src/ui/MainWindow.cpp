#include "anilex/ui/MainWindow.h"

MainWindow::MainWindow(const QList<Tab> &tabs, QWidget *parent) : QMainWindow(parent) {
    // 1. Central Widget erstellen und setzen
    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    // 2. Layout auf das Central Widget setzen, nicht auf das QMainWindow
    this->layout = new QVBoxLayout(centralWidget);

    // Margins auf 0 setzen, damit der TabBar direkt am Rand klebt
    this->layout->setContentsMargins(0, 0, 0, 0);
    this->layout->setSpacing(0);

    this->tabBar = new TabBar(tabs, this);

    this->setupUI();
    this->setupConnections();
}

void MainWindow::setupUI() {
    // KEIN Qt::AlignTop hier verwenden, wenn es die volle Breite füllen soll
    this->layout->addWidget(this->tabBar);

    // Wenn du willst, dass der TabBar oben bleibt und darunter Platz für anderes ist:
    // Nutze einen Spacer am Ende
    this->layout->addStretch(1);
}

void MainWindow::setupConnections() {
}
