#pragma once
#include <QPropertyAnimation>
#include <QPushButton>

#include "anilex/core/GlobalSettings.h"
#include "anilex/core/TabMeta.h"

/**
 * @class AbstractTab
 * @brief Abstract class of the Tab
 *
 *  This is the base abstract class for all TabBars in this AniLex Project.
 *  It extends the QPushButton class of Qt. This class gives a solid start for animations, function and
 *  variables
 *
 * @note The following methods are pure virtual and must be implemented by subclasses:
 * - @ref setupUi()
 * - @ref startAnimation()
 * - @ref endAnimation()
 * - @ref animationInstant()
 */
class AbstractTab : public QPushButton {
    Q_OBJECT
public:
    /**
     * @param text text of the tab
     * @param parent parent element
     */
    explicit AbstractTab(const QString &text, QWidget *parent = nullptr) : QPushButton(text, parent) {}
    virtual ~AbstractTab() = default;

protected:
    virtual void setupUi() = 0;

    /**
     * @details Creates a standardized animation for a tabs to animate
     * @param name name of the animation to animate
     * @param start start value of the animation
     * @param end end value of the animation
     * @return QPropertyAnimation
     * @note default animation duration to 500 and EasingCurve to OutCubic if not specified in the settings
     */
    virtual QPropertyAnimation *createAnimation(const QByteArray &name, const int start, const int end) {
        QPropertyAnimation *animation = new QPropertyAnimation(this, name, this);
        animation->setDuration(GlobalSettings::instance().value("Animation/duration", 500).toInt());
        animation->setStartValue(start);
        animation->setEndValue(end);
        animation->setEasingCurve(GlobalSettings::instance().value("Animation/EasingCurve",
            QEasingCurve::OutCubic).toEasingCurve());
        return animation;
    }

public slots:
    /** @brief triggers the entry animation of the tab */
    virtual void startAnimation() = 0;
    /** @brief triggers the exit animation of the tab */
    virtual void endAnimation() = 0;
    /** @brief sets the tab to the final animated state */
    virtual void animationInstant() = 0;
};
