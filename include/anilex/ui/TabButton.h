#pragma once
#include <QPropertyAnimation>
#include <QSvgRenderer>
#include <QResizeEvent>
#include <QPushButton>
#include <QString>
#include "interfaces/AbstractTab.h"

/**
 * @class TabButton
 * @brief A self-made TabButton with custom animations and support for SVG Icons
 *
 * This class implements the @ref AbstractTab interface. It uses Q_PROPERTY to make the animations for the
 * tabs smooth for opacity, vertical movement and resizing it
 *
 * @note rendering is handled int custom @ref paintEvent()
 */
class TabButton : public AbstractTab {
    Q_OBJECT
    /** @property dy: property for the vertical movement **/
    Q_PROPERTY(int dy READ getDy WRITE setDy)
    /** @property alpha: property for making the circle visible and invisible **/
    Q_PROPERTY(int alpha READ getAlpha WRITE setAlpha)
    /** @property dy: property for the circle bigger when moving down **/
    Q_PROPERTY(double sizeDiff READ getSizeDiff WRITE setSizeDiff)

private:
    QSvgRenderer *renderer = nullptr;
    double sizeDiff = 0.0; // modifier for the making the circle bigger

    int dy = 0; // modifier for the y-axis moving of the circle
    int alpha = 0; // modifier for the alpha channel for the circle

public:
    /**
     * @param text Text which is shown
     * @param icon path to the icon which should be displayed
     * @param parent parent widget
     */
    explicit TabButton(const QString &text, const QString &icon, QWidget *parent = nullptr);

    /**
     * @name property accessors
     * Required for QPropertyAnimations
     * @{ */
    void setDy(int val) { dy = val; update(); }
    int getDy() const { return dy; }

    void setAlpha(int val) { alpha = val; update(); }
    int getAlpha() const { return alpha; }

    void setSizeDiff(double val) { sizeDiff = val; update(); }
    double getSizeDiff() const { return sizeDiff; }
    /** @} */

protected:
    /**
     * @brief custom paint event to draw the svg and draw a circle around the picture
     */
    void paintEvent(QPaintEvent* event) override;
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