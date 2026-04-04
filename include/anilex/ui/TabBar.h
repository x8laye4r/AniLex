#pragma once
#include <QFrame>
#include <QList>
#include <QHBoxLayout>
#include <QButtonGroup>
#include "TabButton.h"
#include "anilex/core/TabMeta.h"
#include "anilex/ui/interfaces/AbstractTabBar.h"

/**
 * @class TabBar
 * @brief Concrete implementation of a custom TabBar using @ref TabButton
 *
 * This class handles the actual rendering and interactions logic for the tabs.
 * Manages animations between switching to tabs
 *
 * @note implementation is specifically designed to work with @ref TabButton
 * @todo fix animation when switching rapidly between tabs
 */
class TabBar : public AbstractTabBar {
    Q_OBJECT
private:
    int pendingIndex = 0;

protected:
    /** @brief initialized the UI */
    void setupUi() override;
    /** @brief handles all the animations */
    void setupConnections() override;
    /** @brief handles when a tab is toggled */
    void onButtonClicked(QAbstractButton* btn, bool checked) override;

public:
    /**
     * @param tabs tabs which should be rendered
     * @param parent parent widget
     */
    explicit TabBar(const QList<TabMeta> &tabs, QWidget *parent = nullptr);
    void addTab(const TabMeta &tab, int index) override;
    void addTabs(const QList<TabMeta> &tabs) override;
};
