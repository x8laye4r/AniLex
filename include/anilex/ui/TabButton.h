#pragma once
#include <QPropertyAnimation>
#include <QSvgRenderer>
#include <QResizeEvent>
#include <QPushButton>
#include <QString>

/**
 * @class TabButton
 * @brief A self-made TabButton with custom animations with SVG Icons support. Is used for the TabBar
 *
 * The class is using the Q_PROPERTY for the animations so the buttons resize and move down by clicking them.
 * It is also resizing itself via the height of the widget
 *
 * @note Is painted in a custom paintEvent()
 *
 */
class TabButton : public QPushButton {
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

    QPropertyAnimation* createAnimation(const QByteArray &name, const int start, const int end);
    void setupUi(const QString &text);

    int dy = 0; // modifier for the y-axis moving of the circle
    int alpha = 0; // modifier for the alpha channel for the circle
    const int ANIMATION_DURATION;

    bool active = false;

public:
    /**
     * @param text Text which is shown
     * @param icon path to the icon which should be displayed
     * @param animation_duration duration of the animation
     * @param parent parent widget
     */
    explicit TabButton(const QString &text, const QString &icon, const int animation_duration = 500,
                       QWidget *parent = nullptr);

    // needed for the Q_PROPERTY so the animation works
    void setDy(int val) { dy = val; update(); }
    int getDy() const { return dy; }

    void setAlpha(int val) { alpha = val; update(); }
    int getAlpha() const { return alpha; }

    void setSizeDiff(double val) { sizeDiff = val; update(); }
    double getSizeDiff() const { return sizeDiff; }


    void toggleActive() { active = !active; }
    bool isActive() const { return active; }

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

public slots:
    void moveDown();
    void reset();
};