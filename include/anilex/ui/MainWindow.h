#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "Collapsable.h"
#include "TabBar.h"


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const QList<Tab> &tabs, QWidget *parent = nullptr);

protected:
    QString currentUser = nullptr;
    int userId = 0;

private:
    QVBoxLayout *layout = nullptr;
    TabBar *tabBar = nullptr;

    void setupUI();
    void setupConnections();
};

