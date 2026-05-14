#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPropertyAnimation>

#include "anilex/ui/Collapsable.h"

namespace {
constexpr int HOVER_LIGHTNESS_FACTOR = 115;
constexpr int MIN_ICON_SIDE = 12;
constexpr int ICON_SIDE_PADDING = 6;

QString iconForState(Section::IconStyle style, bool expanded, bool useAlternateChevronGlyph) {
    switch (style) {
        case Section::IconStyle::Arrow:
            return expanded ? QStringLiteral("▼") : QStringLiteral("▶");
        case Section::IconStyle::Chevron: {
            if (expanded) {
                return useAlternateChevronGlyph ? QStringLiteral("▾") : QStringLiteral("⌄");
            }
            return useAlternateChevronGlyph ? QStringLiteral("▸") : QStringLiteral("›");
        }
        case Section::IconStyle::PlusMinus:
            return expanded ? QStringLiteral("−") : QStringLiteral("+");
        default:
            return expanded ? QStringLiteral("▼") : QStringLiteral("▶");
    }
}
}

Section::Section(const QString &title, const int animationDuration, QWidget *parent)
    : Section(title, StyleOptions{}, animationDuration, parent) {
}

Section::Section(const QString &title, const StyleOptions &styleOptions, const int animationDuration, QWidget *parent)
    : QWidget(parent), animationDuration(animationDuration), title(title),
      headerBackgroundColor(styleOptions.headerBackgroundColor),
      headerTextColor(styleOptions.headerTextColor),
      contentTextColor(styleOptions.contentTextColor),
      borderColor(styleOptions.borderColor), borderWidth(styleOptions.borderWidth),
      borderRadius(styleOptions.borderRadius), sectionSpacing(styleOptions.sectionSpacing),
      headerHeight(styleOptions.headerHeight), iconColor(styleOptions.iconColor),
      iconSize(styleOptions.iconSize), iconStyle(styleOptions.iconStyle),
      shadowEnabled(styleOptions.shadowEnabled),
      shadowBlurRadius(styleOptions.shadowBlurRadius),
      hoverEffectEnabled(styleOptions.hoverEffectEnabled),
      iconAnimationEnabled(styleOptions.iconAnimationEnabled),
      contentMargins(styleOptions.contentMargins), contentSpacing(styleOptions.contentSpacing),
      headerFont(styleOptions.headerFont), contentFont(styleOptions.contentFont) {
    toggleButton = new QToolButton(this);
    headerLine = new QFrame(this);
    toggleAnimation = new QParallelAnimationGroup(this);
    contentArea = new QScrollArea(this);
    contentWidget = new QWidget(contentArea);
    mainLayout = new QGridLayout(this);

    toggleButton->setObjectName(QStringLiteral("sectionToggleButton"));
    toggleButton->setCheckable(true);
    toggleButton->setChecked(false);
    toggleButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

    headerLine->setFrameShape(QFrame::HLine);
    headerLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    headerLine->setObjectName(QStringLiteral("sectionHeaderLine"));

    contentArea->setObjectName(QStringLiteral("sectionContentArea"));
    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    contentArea->setMaximumHeight(0);
    contentArea->setMinimumHeight(0);
    contentArea->setWidgetResizable(true);
    contentArea->setFrameShape(QFrame::NoFrame);

    contentWidget->setObjectName(QStringLiteral("sectionContentWidget"));
    contentArea->setWidget(contentWidget);

    toggleAnimation->addAnimation(new QPropertyAnimation(this, "minimumHeight"));
    toggleAnimation->addAnimation(new QPropertyAnimation(this, "maximumHeight"));
    toggleAnimation->addAnimation(new QPropertyAnimation(contentArea, "maximumHeight"));

    mainLayout->setContentsMargins(0, 0, 0, 0);

    int row = 0;
    mainLayout->addWidget(toggleButton, row, 0, 1, 1);
    mainLayout->addWidget(headerLine, row++, 1, 1, 1);
    mainLayout->addWidget(contentArea, row, 0, 1, 2);
    setLayout(mainLayout);

    applyStyle();
    updateHeaderText(false);
    updateAnimationTimings();

    QObject::connect(toggleButton, &QToolButton::clicked, this, [this](const bool checked) {
        updateHeaderText(checked);
        toggleAnimation->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        toggleAnimation->start();
    });
}

void Section::setContentLayout(QLayout &contentLayout) {
    if (contentWidget->layout() != nullptr) {
        delete contentWidget->layout();
    }

    contentWidget->setLayout(&contentLayout);
    contentLayout.setContentsMargins(contentMargins);
    contentLayout.setSpacing(contentSpacing);
    refreshContentMetrics();
}

int Section::getAnimationDuration() const {
    return animationDuration;
}

void Section::setAnimationDuration(int duration) {
    animationDuration = qMax(0, duration);
    updateAnimationTimings();
}

QColor Section::getHeaderBackgroundColor() const {
    return headerBackgroundColor;
}

void Section::setHeaderBackgroundColor(const QColor &color) {
    headerBackgroundColor = color;
    applyStyle();
}

QColor Section::getHeaderTextColor() const {
    return headerTextColor;
}

void Section::setHeaderTextColor(const QColor &color) {
    headerTextColor = color;
    applyStyle();
}

QColor Section::getContentTextColor() const {
    return contentTextColor;
}

void Section::setContentTextColor(const QColor &color) {
    contentTextColor = color;
    applyStyle();
}

QColor Section::getBorderColor() const {
    return borderColor;
}

void Section::setBorderColor(const QColor &color) {
    borderColor = color;
    applyStyle();
}

int Section::getBorderWidth() const {
    return borderWidth;
}

void Section::setBorderWidth(int width) {
    borderWidth = qMax(0, width);
    applyStyle();
}

int Section::getBorderRadius() const {
    return borderRadius;
}

void Section::setBorderRadius(int radius) {
    borderRadius = qMax(0, radius);
    applyStyle();
}

int Section::getSectionSpacing() const {
    return sectionSpacing;
}

void Section::setSectionSpacing(int spacing) {
    sectionSpacing = qMax(0, spacing);
    mainLayout->setVerticalSpacing(sectionSpacing);
    refreshContentMetrics();
}

QMargins Section::getContentMargins() const {
    return contentMargins;
}

void Section::setContentMargins(const QMargins &margins) {
    contentMargins = margins;
    if (contentWidget->layout() != nullptr) {
        contentWidget->layout()->setContentsMargins(contentMargins);
    }
    refreshContentMetrics();
}

int Section::getContentSpacing() const {
    return contentSpacing;
}

void Section::setContentSpacing(int spacing) {
    contentSpacing = qMax(0, spacing);
    if (contentWidget->layout() != nullptr) {
        contentWidget->layout()->setSpacing(contentSpacing);
    }
    refreshContentMetrics();
}

int Section::getHeaderHeight() const {
    return headerHeight;
}

void Section::setHeaderHeight(int height) {
    headerHeight = qMax(0, height);
    applyStyle();
    refreshContentMetrics();
}

QColor Section::getIconColor() const {
    return iconColor;
}

void Section::setIconColor(const QColor &color) {
    iconColor = color;
    applyStyle();
}

int Section::getIconSize() const {
    return iconSize;
}

void Section::setIconSize(int size) {
    iconSize = qMax(1, size);
    applyStyle();
    updateHeaderText(toggleButton->isChecked());
}

Section::IconStyle Section::getIconStyle() const {
    return iconStyle;
}

void Section::setIconStyle(IconStyle style) {
    iconStyle = style;
    updateHeaderText(toggleButton->isChecked());
}

bool Section::isShadowEnabled() const {
    return shadowEnabled;
}

void Section::setShadowEnabled(bool enabled) {
    shadowEnabled = enabled;
    applyStyle();
}

int Section::getShadowBlurRadius() const {
    return shadowBlurRadius;
}

void Section::setShadowBlurRadius(int radius) {
    shadowBlurRadius = qMax(0, radius);
    applyStyle();
}

bool Section::isHoverEffectEnabled() const {
    return hoverEffectEnabled;
}

void Section::setHoverEffectEnabled(bool enabled) {
    hoverEffectEnabled = enabled;
    applyStyle();
}

bool Section::isIconAnimationEnabled() const {
    return iconAnimationEnabled;
}

void Section::setIconAnimationEnabled(bool enabled) {
    iconAnimationEnabled = enabled;
    updateHeaderText(toggleButton->isChecked());
}

QFont Section::getHeaderFont() const {
    return headerFont;
}

void Section::setHeaderFont(const QFont &font) {
    headerFont = font;
    applyStyle();
}

QFont Section::getContentFont() const {
    return contentFont;
}

void Section::setContentFont(const QFont &font) {
    contentFont = font;
    applyStyle();
}

void Section::applyStyle() {
    toggleButton->setFont(headerFont);
    contentWidget->setFont(contentFont);

    mainLayout->setVerticalSpacing(sectionSpacing);
    toggleButton->setMinimumHeight(headerHeight);
    toggleButton->setMaximumHeight(headerHeight);

    const QString headerBg = headerBackgroundColor.name(QColor::HexArgb);
    const QString headerFg = headerTextColor.name(QColor::HexArgb);
    const QString contentFg = contentTextColor.name(QColor::HexArgb);
    const QString border = borderColor.name(QColor::HexArgb);
    const QString hoverBg = headerBackgroundColor.lighter(hoverEffectEnabled ? HOVER_LIGHTNESS_FACTOR : 100).name(QColor::HexArgb);

    toggleButton->setStyleSheet(QStringLiteral(
        "QToolButton#sectionToggleButton {"
        "text-align: left;"
        "padding: 0 12px;"
        "border: %1px solid %2;"
        "border-top-left-radius: %3px;"
        "border-top-right-radius: %3px;"
        "background-color: %4;"
        "color: %5;"
        "}"
        "QToolButton#sectionToggleButton:hover {"
        "background-color: %6;"
        "}")
            .arg(borderWidth)
            .arg(border)
            .arg(borderRadius)
            .arg(headerBg)
            .arg(headerFg)
            .arg(hoverBg));

    headerLine->setStyleSheet(QStringLiteral(
        "QFrame#sectionHeaderLine {"
        "border: 0;"
        "border-top: %1px solid %2;"
        "margin-left: 8px;"
        "}")
            .arg(borderWidth)
            .arg(border));

    contentWidget->setStyleSheet(QStringLiteral(
        "QWidget#sectionContentWidget {"
        "color: %1;"
        "border-left: %2px solid %3;"
        "border-right: %2px solid %3;"
        "border-bottom: %2px solid %3;"
        "border-bottom-left-radius: %4px;"
        "border-bottom-right-radius: %4px;"
        "}")
            .arg(contentFg)
            .arg(borderWidth)
            .arg(border)
            .arg(borderRadius));

    if (shadowEnabled) {
        auto *shadow = qobject_cast<QGraphicsDropShadowEffect *>(graphicsEffect());
        if (shadow == nullptr) {
            shadow = new QGraphicsDropShadowEffect(this);
            setGraphicsEffect(shadow);
        }

        shadow->setBlurRadius(shadowBlurRadius);
        shadow->setOffset(0, 4);
        shadow->setColor(QColor(0, 0, 0, 100));
    } else {
        setGraphicsEffect(nullptr);
    }

    updateHeaderText(toggleButton->isChecked());
}

void Section::updateHeaderText(bool expanded) {
    const QString icon = iconForState(iconStyle, expanded, iconAnimationEnabled);
    const int iconSide = qMax(MIN_ICON_SIDE, iconSize + ICON_SIDE_PADDING);
    QPixmap pixmap(iconSide, iconSide);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setPen(iconColor);
    QFont iconFont = headerFont;
    iconFont.setPixelSize(iconSize);
    painter.setFont(iconFont);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, icon);
    painter.end();

    toggleButton->setIcon(QIcon(pixmap));
    toggleButton->setIconSize(pixmap.size());
    toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggleButton->setText(title);
}

void Section::updateAnimationTimings() {
    for (int i = 0; i < toggleAnimation->animationCount(); ++i) {
        if (auto *animation = qobject_cast<QPropertyAnimation *>(toggleAnimation->animationAt(i)); animation != nullptr) {
            animation->setDuration(animationDuration);
        }
    }
}

void Section::refreshContentMetrics() {
    const auto collapsedHeight = sizeHint().height() - contentArea->maximumHeight();
    const auto contentHeight = contentWidget->sizeHint().height();

    for (int i = 0; i < toggleAnimation->animationCount() - 1; ++i) {
        if (auto *sectionAnimation = qobject_cast<QPropertyAnimation *>(toggleAnimation->animationAt(i)); sectionAnimation != nullptr) {
            sectionAnimation->setStartValue(collapsedHeight);
            sectionAnimation->setEndValue(collapsedHeight + contentHeight);
        }
    }

    if (auto *contentAnimation = qobject_cast<QPropertyAnimation *>(toggleAnimation->animationAt(toggleAnimation->animationCount() - 1));
        contentAnimation != nullptr) {
        contentAnimation->setStartValue(0);
        contentAnimation->setEndValue(contentHeight);
    }
}
