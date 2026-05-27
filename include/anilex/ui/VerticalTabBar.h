#pragma once
#include "TabButtonSimple.h"
#include "interfaces/AbstractTabBar.h"

/**
 * @class VerticalTabBar
 * @brief same as the @ref TabBarSimple but in vertical
 *
 * This class implements the @ref AbstractTab interface. It uses Q_PROPERTY to make the animations for the
 * tabs smooth for resizing the font-size
 *
 * @note font resizing is handled in custom @ref resizeEvent()
 * @todo fix animation when switching rapidly between tabs
 */
class VerticalTabBar : public AbstractTabBar {
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
    void updateIndicator(TabButtonSimple * target, bool immediate);

public:
    /**
     * @param tabs tabs which should be rendered
     * @param parent parent widget
     */
    explicit VerticalTabBar(const QList<TabMeta> &tabs, QWidget *parent = nullptr);
    void addTab(const TabMeta &tab, int index) override;
    void addTabs(const QList<TabMeta> &tabs) override;
};
