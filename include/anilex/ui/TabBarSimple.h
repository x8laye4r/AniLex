#pragma once
#include "TabButtonSimple.h"
#include "anilex/ui/interfaces/AbstractTabBar.h"

/**
 * @class TabBarSimple
 * @brief Concrete implementation of a custom TabBar using @ref TabButtonSimple but with more simpler animations
 *
 * This class handles the actual rendering and interactions logic for the tabs.
 * Manages animations between switching to tabs
 *
 * @note implementation is specifically designed to work with @ref TabButton
 * @todo fix animation when switching rapidly between tabs
 */
class TabBarSimple : public AbstractTabBar {
    Q_OBJECT
private:
    /** @brief the indicator showing which tab is active */
    QWidget *indicator = nullptr;

protected:
    /** @brief initialized the UI */
    void setupUi() override;
    /** @brief handles all the animations */
    void setupConnections() override;
    /** @brief handles when a tab is toggled */
    void onButtonClicked(QAbstractButton *button, bool checked) override;
    /** @brief handles when resizing the window so the indicator is in the correct place */
    void resizeEvent(QResizeEvent *event) override;
    /** @brief handles updating the indicator */
    void updateIndicator(TabButtonSimple *tabButton, bool immediate);

public:
    /**
     * @param tabs tabs which should be rendered
     * @param parent parent widget
     */
    explicit TabBarSimple(const QList<TabMeta> &tabs, QWidget *parent = nullptr);
    void addTab(const TabMeta &tab, int index) override;
    void addTabs(const QList<TabMeta> &tabs) override;
};



