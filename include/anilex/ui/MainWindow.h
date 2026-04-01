#pragma once
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedLayout>
#include "Collapsable.h"
#include "TabBar.h"
#include "TabButton.h"

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
    QStackedLayout *stackedLayout = nullptr;

    void setupUI(const QList<Tab> &tabs);
    void setupConnections();

    QWidget* createPage(const QString &text);
};