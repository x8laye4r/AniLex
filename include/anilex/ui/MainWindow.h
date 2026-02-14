#ifndef ANILEX_MAINWINDOW_H
#define ANILEX_MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

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

    void setupUI();
    void setupConnections();

private slots:
    static void onButtonClick();
};


#endif //ANILEX_MAINWINDOW_H