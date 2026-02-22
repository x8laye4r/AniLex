#pragma once

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>


class Collapsable : public QWidget {
    Q_OBJECT
public:
    static const int DEFAULT_DURATION = 0;

    explicit Collapsable(const QString &title = "", const int animationDuration = DEFAULT_DURATION, QWidget *parent = nullptr);

    void setContentLayout(QLayout &contentLayout);

    void setTitle(QString title);

    void updateHeights();

private:
    QGridLayout *mainLayout = nullptr;
    QToolButton *toggleButton = nullptr;
    QFrame *headerLine = nullptr;
    QParallelAnimationGroup *toggleAnimation = nullptr;
    QScrollArea *contentArea = nullptr;
    int animationDuration;
    int collapsedHeight;
    bool isExpanded = false;

public slots:
    void toggle(bool collapsed);
};

