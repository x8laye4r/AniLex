# Section (Collapsable) Widget Usage Guide

The enhanced `Section` widget is a highly customizable collapsible container that supports both creation-time and runtime styling. This guide demonstrates various usage patterns.

## Basic Usage

### Simple Collapsible Section

```cpp
#include "anilex/ui/Collapsable.h"
#include <QVBoxLayout>
#include <QLabel>

// Create a simple section with default styling
auto *section = new Section("Basic Section", 200, parentWidget);

// Add content
auto *layout = new QVBoxLayout();
layout->addWidget(new QLabel("This is collapsible content"));
layout->addWidget(new QLabel("Click the header to expand/collapse"));
section->setContentLayout(*layout);
```

## Creation-Time Customization

### Dark Theme Section with Custom Colors

```cpp
Section::StyleOptions darkStyle;
darkStyle.headerBackgroundColor = QColor("#1F2937");
darkStyle.headerTextColor = QColor("#E5E7EB");
darkStyle.contentTextColor = QColor("#D1D5DB");
darkStyle.borderColor = QColor("#4B5563");
darkStyle.borderWidth = 2;
darkStyle.borderRadius = 12;
darkStyle.headerHeight = 44;
darkStyle.iconColor = QColor("#60A5FA");
darkStyle.iconSize = 16;
darkStyle.iconStyle = Section::IconStyle::Chevron;
darkStyle.shadowEnabled = true;
darkStyle.shadowBlurRadius = 20;
darkStyle.hoverEffectEnabled = true;
darkStyle.contentMargins = QMargins(12, 12, 12, 12);
darkStyle.contentSpacing = 10;

auto *section = new Section("Dark Theme Section", darkStyle, 300, parentWidget);
```

### Light Theme Section

```cpp
Section::StyleOptions lightStyle;
lightStyle.headerBackgroundColor = QColor("#F3F4F6");
lightStyle.headerTextColor = QColor("#111827");
lightStyle.contentTextColor = QColor("#374151");
lightStyle.borderColor = QColor("#D1D5DB");
lightStyle.borderWidth = 1;
lightStyle.borderRadius = 8;
lightStyle.headerHeight = 40;
lightStyle.iconColor = QColor("#6366F1");
lightStyle.iconSize = 14;
lightStyle.iconStyle = Section::IconStyle::Arrow;
lightStyle.shadowEnabled = false;
lightStyle.hoverEffectEnabled = true;

auto *section = new Section("Light Theme Section", lightStyle, 250, parentWidget);
```

### Plus/Minus Icon Style

```cpp
Section::StyleOptions plusMinusStyle;
plusMinusStyle.iconStyle = Section::IconStyle::PlusMinus;
plusMinusStyle.iconColor = QColor("#10B981");
plusMinusStyle.iconSize = 18;
plusMinusStyle.headerBackgroundColor = QColor("#ECFDF5");
plusMinusStyle.headerTextColor = QColor("#065F46");
plusMinusStyle.borderColor = QColor("#A7F3D0");

auto *section = new Section("Settings", plusMinusStyle, 200, parentWidget);
```

## Runtime Customization (Settings Integration)

### Updating Colors from Settings

```cpp
// Load colors from user settings
QColor headerBg = settings.value("UI/SectionHeaderBg", QColor("#27272A")).value<QColor>();
QColor headerText = settings.value("UI/SectionHeaderText", QColor("#F4F4F5")).value<QColor>();
QColor contentText = settings.value("UI/SectionContentText", QColor("#E4E4E7")).value<QColor>();

// Apply to existing section
section->setHeaderBackgroundColor(headerBg);
section->setHeaderTextColor(headerText);
section->setContentTextColor(contentText);
```

### Adjusting Layout and Spacing

```cpp
// Customize spacing and margins
section->setContentMargins(QMargins(8, 8, 8, 8));
section->setContentSpacing(6);
section->setSectionSpacing(2);
section->setHeaderHeight(38);
```

### Changing Icon Style at Runtime

```cpp
// Switch between icon styles
section->setIconStyle(Section::IconStyle::Chevron);
section->setIconColor(QColor("#F59E0B"));
section->setIconSize(16);
section->setIconAnimationEnabled(true);
```

### Toggling Visual Effects

```cpp
// Enable/disable hover effects
section->setHoverEffectEnabled(true);

// Enable/disable shadow
section->setShadowEnabled(true);
section->setShadowBlurRadius(24);

// Control animation speed
section->setAnimationDuration(400);
```

### Font Customization

```cpp
// Customize header font
QFont headerFont("Sans", 11, QFont::Bold);
headerFont.setItalic(true);
section->setHeaderFont(headerFont);

// Customize content font
QFont contentFont("Monospace", 9, QFont::Normal);
section->setContentFont(contentFont);
```

## Complete Settings Integration Example

```cpp
class SettingsPanel : public QWidget {
public:
    SettingsPanel(QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QVBoxLayout(this);
        
        // Create sections with default styling
        auto *appearanceSection = new Section("Appearance", 250, this);
        auto *behaviorSection = new Section("Behavior", 250, this);
        
        // Add content to sections
        setupAppearanceSection(appearanceSection);
        setupBehaviorSection(behaviorSection);
        
        layout->addWidget(appearanceSection);
        layout->addWidget(behaviorSection);
        layout->addStretch();
    }

private:
    void setupAppearanceSection(Section *section) {
        auto *layout = new QVBoxLayout();
        
        // Add color pickers, font selectors, etc.
        auto *colorLabel = new QLabel("Header Color:");
        auto *colorButton = new QPushButton("Choose...");
        
        connect(colorButton, &QPushButton::clicked, this, [section]() {
            QColor color = QColorDialog::getColor(Qt::white, nullptr, "Select Header Color");
            if (color.isValid()) {
                section->setHeaderBackgroundColor(color);
            }
        });
        
        layout->addWidget(colorLabel);
        layout->addWidget(colorButton);
        section->setContentLayout(*layout);
    }
    
    void setupBehaviorSection(Section *section) {
        auto *layout = new QVBoxLayout();
        
        // Add animation speed slider
        auto *speedLabel = new QLabel("Animation Speed (ms):");
        auto *speedSlider = new QSlider(Qt::Horizontal);
        speedSlider->setRange(50, 500);
        speedSlider->setValue(250);
        
        connect(speedSlider, &QSlider::valueChanged, this, [section](int value) {
            section->setAnimationDuration(value);
        });
        
        layout->addWidget(speedLabel);
        layout->addWidget(speedSlider);
        section->setContentLayout(*layout);
    }
};
```

## Available Properties

### Colors
- `headerBackgroundColor` - Background color of the header
- `headerTextColor` - Text color in the header
- `contentTextColor` - Text color in the content area
- `borderColor` - Color of borders
- `iconColor` - Color of the expand/collapse icon

### Dimensions
- `borderWidth` - Width of borders (pixels)
- `borderRadius` - Corner radius (pixels)
- `headerHeight` - Height of the header (pixels)
- `iconSize` - Size of the icon (pixels)
- `sectionSpacing` - Vertical spacing between header and content (pixels)

### Spacing
- `contentMargins` - Margins inside the content area (QMargins)
- `contentSpacing` - Spacing between items in content layout (pixels)

### Fonts
- `headerFont` - Font used for the header text
- `contentFont` - Font used for the content text

### Icons
- `iconStyle` - Icon style (Arrow, Chevron, PlusMinus)
- `iconAnimationEnabled` - Whether to use animated icon variants

### Effects
- `shadowEnabled` - Whether to show drop shadow
- `shadowBlurRadius` - Blur radius of the shadow (pixels)
- `hoverEffectEnabled` - Whether to show hover effect on header
- `animationDuration` - Duration of expand/collapse animation (milliseconds)

## Icon Styles

### Arrow Style
```
Collapsed: ▶
Expanded:  ▼
```

### Chevron Style (with animation variants)
```
Collapsed: ▸ (animated) or › (static)
Expanded:  ▾ (animated) or ⌄ (static)
```

### Plus/Minus Style
```
Collapsed: +
Expanded:  −
```

## Getter Methods

All properties have corresponding getter methods:

```cpp
// Get current values
QColor headerBg = section->getHeaderBackgroundColor();
int headerHeight = section->getHeaderHeight();
QFont headerFont = section->getHeaderFont();
bool shadowEnabled = section->isShadowEnabled();
int animationDuration = section->getAnimationDuration();
// ... and many more
```

## Tips and Best Practices

1. **Use StyleOptions for consistent theming** - Create reusable StyleOptions structs for your application theme
2. **Batch updates** - When changing multiple properties, consider creating a new StyleOptions and using the constructor
3. **Animation duration** - Keep animation duration between 100-500ms for smooth but responsive feel
4. **Icon size** - Recommended range is 12-20 pixels for good visibility
5. **Content margins** - Use 8-12 pixels for comfortable spacing
6. **Shadow effects** - Use shadows sparingly; they work best with light backgrounds
7. **Font selection** - Stick to system fonts (Sans, Serif, Monospace) for cross-platform compatibility

## Example: Theme Switcher

```cpp
class ThemeSwitcher {
public:
    static Section::StyleOptions getDarkTheme() {
        Section::StyleOptions style;
        style.headerBackgroundColor = QColor("#1F2937");
        style.headerTextColor = QColor("#E5E7EB");
        style.contentTextColor = QColor("#D1D5DB");
        style.borderColor = QColor("#4B5563");
        style.shadowEnabled = true;
        return style;
    }
    
    static Section::StyleOptions getLightTheme() {
        Section::StyleOptions style;
        style.headerBackgroundColor = QColor("#F3F4F6");
        style.headerTextColor = QColor("#111827");
        style.contentTextColor = QColor("#374151");
        style.borderColor = QColor("#D1D5DB");
        style.shadowEnabled = false;
        return style;
    }
    
    static void applyTheme(Section *section, bool isDark) {
        auto style = isDark ? getDarkTheme() : getLightTheme();
        section->setHeaderBackgroundColor(style.headerBackgroundColor);
        section->setHeaderTextColor(style.headerTextColor);
        section->setContentTextColor(style.contentTextColor);
        section->setBorderColor(style.borderColor);
        section->setShadowEnabled(style.shadowEnabled);
    }
};
```
