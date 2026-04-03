#include "anilex/ui/TabButton.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

namespace TabBtnConf {
    struct Tab {
        static constexpr float fontSize = 0.18f;
        static constexpr float svgSize = 0.35f;
        static constexpr float textPadding = 0.1f;
        static constexpr float spacing = 0.05f;
        static constexpr float sizeDiff = 0.15f;
        static constexpr float moveDownMultiplier = 0.30f;

        static constexpr int svgMin = 20;
        static constexpr int svgMax = 45;
        static constexpr int textMin = 12;
        static constexpr int textMax = 22;
        static constexpr int heightMin = 75;
        static constexpr int heightMax = 150;
        static constexpr int padding = 8;
    };
}

TabButton::TabButton(const QString &text, const QString &icon, QWidget *parent)
    : AbstractTab(text, parent) {

    renderer = new QSvgRenderer(icon, this);
    this->TabButton::setupUi();
}

void TabButton::setupUi() {
    this->setObjectName("tabButton");
    this->setMinimumHeight(TabBtnConf::Tab::heightMin);
    this->setCheckable(true);
}

void TabButton::resizeEvent(QResizeEvent *event) {
    QPushButton::resizeEvent(event);

    const int fontSize = qBound(TabBtnConf::Tab::textMin, static_cast<int>(this->height() * TabBtnConf::Tab::fontSize),
                                TabBtnConf::Tab::textMax);

    QFont font = this->font();
    font.setPixelSize(fontSize);
    this->setFont(font);

    const int paddingTop = static_cast<int>(this->height() * TabBtnConf::Tab::textPadding);
    this->setStyleSheet(QString("QPushButton#tabButton { padding-top: %1px; text-align: top; }").arg(paddingTop));

    this->setMaximumHeight(qMin(this->width(), TabBtnConf::Tab::heightMax));
}

void TabButton::startAnimation() {
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    const int svgSize = qBound(TabBtnConf::Tab::svgMin, static_cast<int>(this->height() * TabBtnConf::Tab::svgSize),
                               TabBtnConf::Tab::svgMax);
    int targetDy = svgSize * TabBtnConf::Tab::moveDownMultiplier;

    group->addAnimation(createAnimation("dy", dy, targetDy));
    group->addAnimation(createAnimation("alpha", alpha, 255));
    group->addAnimation(createAnimation("sizeDiff", sizeDiff, this->height() * TabBtnConf::Tab::sizeDiff));

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void TabButton::animationInstant() {
    const int svgSize = qBound(TabBtnConf::Tab::svgMin, static_cast<int>(this->height() * TabBtnConf::Tab::svgSize),
                               TabBtnConf::Tab::svgMax);
    int targetDy = svgSize * TabBtnConf::Tab::moveDownMultiplier;

    dy = targetDy;
    alpha = 255;
    sizeDiff = this->height() * TabBtnConf::Tab::sizeDiff;
}

void TabButton::endAnimation() {
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    group->addAnimation(createAnimation("dy", dy, 0));
    group->addAnimation(createAnimation("alpha", alpha, 0));
    group->addAnimation(createAnimation("sizeDiff", sizeDiff, 0));

    group->start(QAbstractAnimation::DeleteWhenStopped);
}


void TabButton::paintEvent(QPaintEvent *event) {
    if (!renderer || !renderer->isValid()) {
        QPushButton::paintEvent(event);
        return;
    }

    QPushButton::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int svgSize = qBound(TabBtnConf::Tab::svgMin, static_cast<int>(this->height() * TabBtnConf::Tab::svgSize),
                               TabBtnConf::Tab::svgMax);

    const int startY = TabBtnConf::Tab::padding + this->fontMetrics().height() + (this->height() * TabBtnConf::Tab::spacing);

    int y = startY + dy;
    int x = (this->width() - svgSize) / 2;

    if (alpha > 0) {
        const double size = svgSize + TabBtnConf::Tab::padding + sizeDiff;
        const double offset = (TabBtnConf::Tab::padding + sizeDiff) / 2.0;
        QRectF bgRect(x - offset, y - offset, size, size);

        QLinearGradient gradient(bgRect.topLeft(), bgRect.bottomLeft());
        QColor c1(0x101010); // TODO Make it so you can choose the colors
        QColor c2(0x191919);
        c1.setAlpha(alpha);
        c2.setAlpha(alpha);

        gradient.setStops({
            {0.0, c1},
            {0.5, c2},
            {1.0, c1}
        });

        painter.setPen(QPen(QColor(255, 255, 255, alpha), 1.0)); // This is needed for drawing the outline

        painter.setBrush(gradient);
        painter.drawEllipse(bgRect);
    }

    renderer->render(&painter, QRectF(x, y, svgSize, svgSize));
}