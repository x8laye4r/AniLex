#pragma once
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>
#include "Collapsable.h"
#include "TabBar.h"
#include "TabButton.h"

class Designer;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const QList<TabMeta> &tabs, QWidget *parent = nullptr);

protected:
    QString currentUser = nullptr;
    int userId = 0;

private:
    void setupUI(const QList<TabMeta> &tabs);
    void setupConnections();
    QWidget* createPage(const QString &text);
    void openDesigner();

    QLayout *layout = nullptr;
    AbstractTabBar *tabBar = nullptr;
    QStackedLayout *stackedLayout = nullptr;
    QPushButton *designerButton = nullptr;
    Designer *designerWindow = nullptr;
};