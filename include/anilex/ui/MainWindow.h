#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "anilex/core/Collapsable.h"


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QVBoxLayout *layout = nullptr;
    QWidget *centralWidget = nullptr;

    QLabel *label = nullptr;
    QPushButton *button = nullptr;

    QLabel *picture_container = nullptr;

    Section *collapsable_ = nullptr;

    QVBoxLayout *layout2 = nullptr;

    void setupUI();
    void setupConnections();

private slots:
    static void onButtonClick();
};

