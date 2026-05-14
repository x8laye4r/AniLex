#pragma once

#include <QColor>
#include <QFont>
#include <QFrame>
#include <QGridLayout>
#include <QMargins>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

/**
 * @class Section
 * @brief Collapsible container with customizable appearance and animation.
 *
 * The widget allows styling both at creation time via @ref StyleOptions and later
 * through dedicated getter/setter methods (e.g. from user UI settings).
 */
class Section : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Available icon presets for expand/collapse state.
     */
    enum class IconStyle {
        Arrow,
        Chevron,
        PlusMinus
    };

    /**
     * @brief Creation-time styling bundle for the section.
     */
    struct StyleOptions {
        QColor headerBackgroundColor = QColor("#27272A");
        QColor headerTextColor = QColor("#F4F4F5");
        QColor contentTextColor = QColor("#E4E4E7");
        QColor borderColor = QColor("#3F3F46");
        int borderWidth = 1;
        int borderRadius = 8;
        int sectionSpacing = 4;
        int headerHeight = 36;
        QColor iconColor = QColor("#F4F4F5");
        int iconSize = 14;
        IconStyle iconStyle = IconStyle::Arrow;
        bool shadowEnabled = false;
        bool hoverEffectEnabled = true;
        bool iconAnimationEnabled = true;
        QMargins contentMargins = QMargins(10, 10, 10, 10);
        int contentSpacing = 8;
        QFont headerFont = QFont(QStringLiteral("Sans Serif"), 10, QFont::DemiBold);
        QFont contentFont = QFont(QStringLiteral("Sans Serif"), 10, QFont::Normal);
    };

private:
    QGridLayout *mainLayout = nullptr;
    QToolButton *toggleButton = nullptr;
    QFrame *headerLine = nullptr;
    QParallelAnimationGroup *toggleAnimation = nullptr;
    QScrollArea *contentArea = nullptr;
    QWidget *contentWidget = nullptr;

    int animationDuration = 100;
    QString title;

    QColor headerBackgroundColor = QColor("#27272A");
    QColor headerTextColor = QColor("#F4F4F5");
    QColor contentTextColor = QColor("#E4E4E7");
    QColor borderColor = QColor("#3F3F46");
    int borderWidth = 1;
    int borderRadius = 8;
    int sectionSpacing = 4;
    int headerHeight = 36;
    QColor iconColor = QColor("#F4F4F5");
    int iconSize = 14;
    IconStyle iconStyle = IconStyle::Arrow;
    bool shadowEnabled = false;
    bool hoverEffectEnabled = true;
    bool iconAnimationEnabled = true;
    QMargins contentMargins = QMargins(10, 10, 10, 10);
    int contentSpacing = 8;
    QFont headerFont = QFont(QStringLiteral("Sans Serif"), 10, QFont::DemiBold);
    QFont contentFont = QFont(QStringLiteral("Sans Serif"), 10, QFont::Normal);

    void applyStyle();
    void updateHeaderText(bool expanded);
    void updateAnimationTimings();
    void refreshContentMetrics();

public:
    explicit Section(const QString &title = "", const int animationDuration = 100, QWidget *parent = nullptr);
    explicit Section(const QString &title, const StyleOptions &styleOptions, const int animationDuration = 100, QWidget *parent = nullptr);

    void setContentLayout(QLayout &contentLayout);

    [[nodiscard]] int getAnimationDuration() const;
    void setAnimationDuration(int duration);

    [[nodiscard]] QColor getHeaderBackgroundColor() const;
    void setHeaderBackgroundColor(const QColor &color);

    [[nodiscard]] QColor getHeaderTextColor() const;
    void setHeaderTextColor(const QColor &color);

    [[nodiscard]] QColor getContentTextColor() const;
    void setContentTextColor(const QColor &color);

    [[nodiscard]] QColor getBorderColor() const;
    void setBorderColor(const QColor &color);

    [[nodiscard]] int getBorderWidth() const;
    void setBorderWidth(int width);

    [[nodiscard]] int getBorderRadius() const;
    void setBorderRadius(int radius);

    [[nodiscard]] int getSectionSpacing() const;
    void setSectionSpacing(int spacing);

    [[nodiscard]] QMargins getContentMargins() const;
    void setContentMargins(const QMargins &margins);

    [[nodiscard]] int getContentSpacing() const;
    void setContentSpacing(int spacing);

    [[nodiscard]] int getHeaderHeight() const;
    void setHeaderHeight(int height);

    [[nodiscard]] QColor getIconColor() const;
    void setIconColor(const QColor &color);

    [[nodiscard]] int getIconSize() const;
    void setIconSize(int size);

    [[nodiscard]] IconStyle getIconStyle() const;
    void setIconStyle(IconStyle style);

    [[nodiscard]] bool isShadowEnabled() const;
    void setShadowEnabled(bool enabled);

    [[nodiscard]] bool isHoverEffectEnabled() const;
    void setHoverEffectEnabled(bool enabled);

    [[nodiscard]] bool isIconAnimationEnabled() const;
    void setIconAnimationEnabled(bool enabled);

    [[nodiscard]] QFont getHeaderFont() const;
    void setHeaderFont(const QFont &font);

    [[nodiscard]] QFont getContentFont() const;
    void setContentFont(const QFont &font);
};
