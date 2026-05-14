#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QColorDialog>

#include "anilex/ui/Collapsable.h"

/**
 * @class SectionUsageExample
 * @brief Demonstrates various usage patterns of the enhanced Section widget
 *
 * This example shows:
 * - Basic section creation
 * - Creation-time customization with StyleOptions
 * - Runtime property updates
 * - Settings integration patterns
 */
class SectionUsageExample : public QWidget {
    Q_OBJECT

public:
    explicit SectionUsageExample(QWidget *parent = nullptr);

private:
    void createBasicExample();
    void createStyledExample();
    void createSettingsExample();
    void createThemeExample();
    void connectSignals();

    Section *basicSection = nullptr;
    Section *styledSection = nullptr;
    Section *settingsSection = nullptr;
    Section *themeSection = nullptr;

    // Settings controls
    QSpinBox *animationSpinBox = nullptr;
    QSlider *headerHeightSlider = nullptr;
    QComboBox *iconStyleCombo = nullptr;
    QCheckBox *shadowCheckBox = nullptr;
    QCheckBox *hoverCheckBox = nullptr;
};

// ============================================================================
// IMPLEMENTATION
// ============================================================================

SectionUsageExample::SectionUsageExample(QWidget *parent)
    : QWidget(parent) {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(12, 12, 12, 12);

    // Example 1: Basic Section
    createBasicExample();
    mainLayout->addWidget(basicSection);

    // Example 2: Styled Section with Custom Colors
    createStyledExample();
    mainLayout->addWidget(styledSection);

    // Example 3: Settings Integration
    createSettingsExample();
    mainLayout->addWidget(settingsSection);

    // Example 4: Theme Switching
    createThemeExample();
    mainLayout->addWidget(themeSection);

    mainLayout->addStretch();

    connectSignals();

    setWindowTitle("Section Widget Usage Examples");
    resize(600, 800);
}

void SectionUsageExample::createBasicExample() {
    // Create a simple section with default styling
    basicSection = new Section("Basic Section", 200, this);

    auto *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("This is a basic collapsible section"));
    layout->addWidget(new QLabel("It uses default styling and colors"));
    layout->addWidget(new QLabel("Click the header to expand/collapse"));
    basicSection->setContentLayout(*layout);
}

void SectionUsageExample::createStyledExample() {
    // Create a section with custom styling
    Section::StyleOptions style;
    style.headerBackgroundColor = QColor("#1F2937");
    style.headerTextColor = QColor("#E5E7EB");
    style.contentTextColor = QColor("#D1D5DB");
    style.borderColor = QColor("#4B5563");
    style.borderWidth = 2;
    style.borderRadius = 12;
    style.headerHeight = 44;
    style.iconColor = QColor("#60A5FA");
    style.iconSize = 16;
    style.iconStyle = Section::IconStyle::Chevron;
    style.shadowEnabled = true;
    style.shadowBlurRadius = 20;
    style.hoverEffectEnabled = true;
    style.contentMargins = QMargins(12, 12, 12, 12);
    style.contentSpacing = 10;

    styledSection = new Section("Styled Section (Dark Theme)", style, 300, this);

    auto *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("This section uses custom styling"));
    layout->addWidget(new QLabel("Dark theme with blue accents"));
    layout->addWidget(new QLabel("Includes drop shadow effect"));
    styledSection->setContentLayout(*layout);
}

void SectionUsageExample::createSettingsExample() {
    // Create a section for runtime customization
    settingsSection = new Section("Runtime Settings", 250, this);

    auto *layout = new QVBoxLayout();

    // Animation duration control
    auto *animLabel = new QLabel("Animation Duration (ms):");
    animationSpinBox = new QSpinBox();
    animationSpinBox->setRange(50, 1000);
    animationSpinBox->setValue(250);
    animationSpinBox->setSingleStep(50);
    layout->addWidget(animLabel);
    layout->addWidget(animationSpinBox);

    // Header height control
    auto *heightLabel = new QLabel("Header Height (px):");
    headerHeightSlider = new QSlider(Qt::Horizontal);
    headerHeightSlider->setRange(30, 60);
    headerHeightSlider->setValue(36);
    layout->addWidget(heightLabel);
    layout->addWidget(headerHeightSlider);

    // Icon style selection
    auto *iconLabel = new QLabel("Icon Style:");
    iconStyleCombo = new QComboBox();
    iconStyleCombo->addItem("Arrow", static_cast<int>(Section::IconStyle::Arrow));
    iconStyleCombo->addItem("Chevron", static_cast<int>(Section::IconStyle::Chevron));
    iconStyleCombo->addItem("Plus/Minus", static_cast<int>(Section::IconStyle::PlusMinus));
    layout->addWidget(iconLabel);
    layout->addWidget(iconStyleCombo);

    // Shadow toggle
    shadowCheckBox = new QCheckBox("Enable Shadow");
    shadowCheckBox->setChecked(false);
    layout->addWidget(shadowCheckBox);

    // Hover effect toggle
    hoverCheckBox = new QCheckBox("Enable Hover Effect");
    hoverCheckBox->setChecked(true);
    layout->addWidget(hoverCheckBox);

    settingsSection->setContentLayout(*layout);
}

void SectionUsageExample::createThemeExample() {
    // Create a section demonstrating theme switching
    themeSection = new Section("Theme Switcher", 250, this);

    auto *layout = new QVBoxLayout();

    auto *darkButton = new QPushButton("Apply Dark Theme");
    auto *lightButton = new QPushButton("Apply Light Theme");
    auto *customButton = new QPushButton("Apply Custom Theme");

    connect(darkButton, &QPushButton::clicked, this, [this]() {
        // Dark theme
        themeSection->setHeaderBackgroundColor(QColor("#1F2937"));
        themeSection->setHeaderTextColor(QColor("#E5E7EB"));
        themeSection->setContentTextColor(QColor("#D1D5DB"));
        themeSection->setBorderColor(QColor("#4B5563"));
        themeSection->setShadowEnabled(true);
    });

    connect(lightButton, &QPushButton::clicked, this, [this]() {
        // Light theme
        themeSection->setHeaderBackgroundColor(QColor("#F3F4F6"));
        themeSection->setHeaderTextColor(QColor("#111827"));
        themeSection->setContentTextColor(QColor("#374151"));
        themeSection->setBorderColor(QColor("#D1D5DB"));
        themeSection->setShadowEnabled(false);
    });

    connect(customButton, &QPushButton::clicked, this, [this]() {
        // Custom theme with user-selected colors
        QColor headerBg = QColorDialog::getColor(Qt::white, this, "Select Header Background");
        if (headerBg.isValid()) {
            themeSection->setHeaderBackgroundColor(headerBg);
        }
    });

    layout->addWidget(darkButton);
    layout->addWidget(lightButton);
    layout->addWidget(customButton);

    themeSection->setContentLayout(*layout);
}

void SectionUsageExample::connectSignals() {
    // Connect settings controls to the settings section
    connect(animationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            settingsSection, &Section::setAnimationDuration);

    connect(headerHeightSlider, &QSlider::valueChanged,
            settingsSection, &Section::setHeaderHeight);

    connect(iconStyleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                auto style = static_cast<Section::IconStyle>(iconStyleCombo->itemData(index).toInt());
                settingsSection->setIconStyle(style);
            });

    connect(shadowCheckBox, &QCheckBox::toggled,
            settingsSection, &Section::setShadowEnabled);

    connect(hoverCheckBox, &QCheckBox::toggled,
            settingsSection, &Section::setHoverEffectEnabled);
}
