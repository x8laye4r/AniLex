#pragma once
#include <QFrame>
#include <QWidget>
#include "AbstractTab.h"

/**
 * @class AbstractTabBar
 * @brief Abstract base class for all tab-bars in the project.
 *
 * This class provides a template for managing layouts, backgrounds, and button grouping
 * using the @ref AbstractTab interface. Concrete button types and specific layout
 * configurations must be defined in derived classes.
 *
 * @note The following methods are pure virtual and must be implemented by subclasses:
 * - @ref setupUi()
 * - @ref setupConnections()
 * - @ref onButtonClicked()
 * - @ref addTab()
 *
 * Most internal Qt pointers are initialized to @c nullptr in the header.
 */
class AbstractTabBar : public QWidget {
    Q_OBJECT
protected:
    QList<AbstractTab*> tabButtons;
    QFrame *container = nullptr;
    QLayout *layout = nullptr;
    QButtonGroup *buttonGroup = nullptr;
    QTimer *timer = nullptr;

    virtual void setupUi() = 0;
    virtual void setupConnections() = 0;
    virtual void onButtonClicked(QAbstractButton* button, bool checked) = 0;

public:
    /**
     * @param parent parent element
     */
    explicit AbstractTabBar(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~AbstractTabBar() = default;
    virtual void addTab(const TabMeta &tab, int index) = 0;
    virtual void addTabs(const QList<TabMeta> &tabs) = 0;

signals:
    /**
     * @brief Emitted when a tab is changed
     * @param index index of the newly selected tab
     */
    void tabChanged(int index);
};
