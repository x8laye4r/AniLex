#include "anilex/ui/MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setupUI();
    this->setupConnections();
}

void MainWindow::setupUI() {
    this->centralWidget = new QWidget(this);
    this->setCentralWidget(this->centralWidget);

    this->layout = new QVBoxLayout(centralWidget);

    this->label = new QLabel("Test Label", centralWidget);
    this->button = new QPushButton("Click for Testing", centralWidget);

    this->picture_container = new QLabel(centralWidget);
    QPixmap pm(":/assets/images/ShiinaV2.jpg");
    if (pm.isNull()) {
        qInfo() << "Pixmap could not be loaded!";
    }
    this->picture_container->setPixmap(pm);

    // this->label->setAlignment(Qt::AlignTop | Qt::AlignRight);
    this->layout->addWidget(label);
    this->layout->addWidget(button);
    this->layout->addWidget(picture_container);

    this->layout->setAlignment(Qt::AlignCenter);
}

void MainWindow::setupConnections() {
    connect(this->button, &QPushButton::clicked, this, &MainWindow::onButtonClick);
}

void MainWindow::onButtonClick() {
    qInfo() << "Button was Clicked";
}
