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

    collapsable_ = new Section(tr("Section"), 300, this);

    this->layout2 = new QVBoxLayout();

    this->layout2->addWidget(new QLabel(tr("Some Text in Section"), collapsable_));
    this->layout2->addWidget(new QPushButton(tr("Button in Section"), collapsable_));

    collapsable_->setContentLayout(*this->layout2);

    this->layout->addWidget(label);
    this->layout->addWidget(button);
    this->layout->addWidget(picture_container);
    this->layout->addWidget(collapsable_);

    this->layout->setAlignment(Qt::AlignCenter);
}

void MainWindow::setupConnections() {
    connect(this->button, &QPushButton::clicked, this, &MainWindow::onButtonClick);
}

void MainWindow::onButtonClick() {
    qInfo() << "Button was Clicked";
}
