#pragma once
#include "interfaces/AbstractTab.h"

/**
 * @class TabButtonSimple
 * @brief A self-made TabButton with custom animations for the font-size
 *
 * This class implements the @ref AbstractTab interface. It uses Q_PROPERTY to make the animations for the
 * tabs smooth for resizing the font-size
 *
 * @note font resizing is handled in custom @ref resizeEvent()
 * @todo fix animation when switching rapidly between tabs
 */
class TabButtonSimple : public AbstractTab {
    Q_OBJECT
    /** @property fontSize: property for resizing the font-size */
    Q_PROPERTY(double fontSize READ getFontSize WRITE setFontSize)
private:
    double fontSize;
public:
    TabButtonSimple(const QString &text, QWidget *parent);

    /**
     * @name property accessors
     * Required for QPropertyAnimations
     * @{ */
    void setFontSize(double val);
    double getFontSize() const { return fontSize; }
    /** @} */

protected:
    /** handles the rescaling of the font and height */
    void resizeEvent(QResizeEvent* event) override;
    /** @brief is setting up the ui */
    void setupUi() override;
public slots:
    /** @brief triggers the entry animation of the tab */
    void startAnimation() override;
    /** @brief triggers the exit animation of the tab */
    void endAnimation() override;
    /** @brief sets the tab to the final animated state */
    void animationInstant() override;
};



