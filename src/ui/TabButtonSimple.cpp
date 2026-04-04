#include "anilex/ui/TabButtonSimple.h"

#include <QPainter>
#include <QParallelAnimationGroup>

namespace TabBtnConf {
    struct Tab {
        static constexpr float fontSize = 0.05f;

        static constexpr int textMin = 12;
        static constexpr int textMax = 18;
        static constexpr int textAnimMax = 18;
        static constexpr int heightMin = 75;
        static constexpr int heightMax = 150;
    };
}

TabButtonSimple::TabButtonSimple(const QString &text, QWidget *parent)
    : AbstractTab(text, parent) {
    this->TabButtonSimple::setupUi();
    fontSize = TabBtnConf::Tab::textMin;
}

void TabButtonSimple::setFontSize(double val) {
    if (fontSize != val) {
        fontSize = val;
        QFont font = this->font();
        font.setPointSizeF(fontSize);
        this->setFont(font);
        this->update();
    }
}

void TabButtonSimple::setupUi() {
    this->setObjectName("tabButtonSimple");
    this->setMinimumHeight(TabBtnConf::Tab::heightMin);
    this->setCheckable(true);
    fontSize = TabBtnConf::Tab::textMin;
    QFont font = this->font();
    font.setPointSizeF(fontSize);
    this->setFont(font);
}

void TabButtonSimple::resizeEvent(QResizeEvent *event) {
    AbstractTab::resizeEvent(event);

    if (!this->isChecked())
        fontSize = qBound(TabBtnConf::Tab::textMin, static_cast<int>(this->height() * TabBtnConf::Tab::fontSize),
                                TabBtnConf::Tab::textMax);

    QFont font = this->font();
    font.setPointSizeF(fontSize);
    this->setFont(font);
    this->setMaximumHeight(qMin(this->width(), TabBtnConf::Tab::heightMax));
}

void TabButtonSimple::startAnimation() {
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    group->addAnimation(createAnimation("fontSize", fontSize, TabBtnConf::Tab::textAnimMax));

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void TabButtonSimple::endAnimation() {
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

    const int temp = qBound(TabBtnConf::Tab::textMin, static_cast<int>(this->height() * TabBtnConf::Tab::fontSize),
                                TabBtnConf::Tab::textMax);

    group->addAnimation(createAnimation("fontSize", fontSize, temp));

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void TabButtonSimple::animationInstant() {
    fontSize = TabBtnConf::Tab::textAnimMax;
}




