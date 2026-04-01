#pragma once
#include <QFrame>
#include <QList>
#include <QHBoxLayout>
#include <QButtonGroup>
#include "TabButton.h"
#include "anilex/core/Tabs.h"

/**
 * @class TabBar
 * @brief A self-made tabbar which is using the @class TabButton to make it a fully usable TabBar
 *
 * @note is currently only usable with the @class TabButton
 */
class TabBar : public QWidget {
    Q_OBJECT
private:
    QList<TabButton*> tabButtons;
    QFrame *container = nullptr;
    QHBoxLayout *layout = nullptr;
    QButtonGroup *buttonGroup = nullptr;

    void setupUi();
    void setupConnections();
    void addTabs(const QList<Tab> &tabs);
    void onButtonToggled(QAbstractButton* btn, bool checked);

    const uint animation_duration;

public:
    /**
     * @param tabs tabs which should be rendered
     * @param animation_duration the duration of the tab switching animation
     * @param parent parent widget
     */
    explicit TabBar(const QList<Tab> &tabs, const uint animation_duration = 500, QWidget *parent = nullptr);
    void addTab(const Tab &tab, int index);

    signals:
        void tabChanged(int index);
};
