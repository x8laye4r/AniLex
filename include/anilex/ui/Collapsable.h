#pragma once
#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

/**
 *
 */
class Section : public QWidget {
    Q_OBJECT
public:
    explicit Section(const QString & title = "", const int animationDuration = 100, QWidget* parent = nullptr);
    void setContentLayout(QLayout & contentLayout);
    void setTitle(const QString &title) const;

private:
    QGridLayout* mainLayout = nullptr;
    QToolButton* toggleButton = nullptr;
    QFrame* headerLine = nullptr;
    QParallelAnimationGroup* toggleAnimation = nullptr;
    QScrollArea* contentArea = nullptr;
    int animationDuration;
};
