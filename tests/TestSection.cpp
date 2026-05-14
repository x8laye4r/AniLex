#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QtTest>

#include "anilex/ui/Collapsable.h"

class TestSection : public QObject {
    Q_OBJECT

private slots:
    void styleOptions_areApplied();
    void setters_updateRuntimeConfiguration();
};

void TestSection::styleOptions_areApplied() {
    Section::StyleOptions style;
    style.headerBackgroundColor = QColor("#1F2937");
    style.headerTextColor = QColor("#E5E7EB");
    style.contentTextColor = QColor("#D1D5DB");
    style.borderColor = QColor("#4B5563");
    style.borderWidth = 2;
    style.borderRadius = 12;
    style.headerHeight = 44;
    style.iconColor = QColor("#60A5FA");
    style.iconSize = 18;
    style.iconStyle = Section::IconStyle::PlusMinus;
    style.shadowEnabled = true;
    style.shadowBlurRadius = 22;
    style.hoverEffectEnabled = false;
    style.iconAnimationEnabled = false;
    style.sectionSpacing = 6;
    style.contentMargins = QMargins(4, 5, 6, 7);
    style.contentSpacing = 11;

    QFont headerFont(QStringLiteral("Sans"), 11, QFont::Bold);
    headerFont.setItalic(true);
    style.headerFont = headerFont;
    style.contentFont = QFont(QStringLiteral("Sans"), 9, QFont::Normal);

    Section section(QStringLiteral("Details"), style, 320);

    QCOMPARE(section.getAnimationDuration(), 320);
    QCOMPARE(section.getHeaderBackgroundColor(), style.headerBackgroundColor);
    QCOMPARE(section.getHeaderTextColor(), style.headerTextColor);
    QCOMPARE(section.getContentTextColor(), style.contentTextColor);
    QCOMPARE(section.getBorderColor(), style.borderColor);
    QCOMPARE(section.getBorderWidth(), style.borderWidth);
    QCOMPARE(section.getBorderRadius(), style.borderRadius);
    QCOMPARE(section.getHeaderHeight(), style.headerHeight);
    QCOMPARE(section.getIconColor(), style.iconColor);
    QCOMPARE(section.getIconSize(), style.iconSize);
    QCOMPARE(section.getIconStyle(), style.iconStyle);
    QCOMPARE(section.isShadowEnabled(), style.shadowEnabled);
    QCOMPARE(section.getShadowBlurRadius(), style.shadowBlurRadius);
    QCOMPARE(section.isHoverEffectEnabled(), style.hoverEffectEnabled);
    QCOMPARE(section.isIconAnimationEnabled(), style.iconAnimationEnabled);
    QCOMPARE(section.getSectionSpacing(), style.sectionSpacing);
    QCOMPARE(section.getContentMargins(), style.contentMargins);
    QCOMPARE(section.getContentSpacing(), style.contentSpacing);
    QCOMPARE(section.getHeaderFont(), style.headerFont);
    QCOMPARE(section.getContentFont(), style.contentFont);

    auto *button = section.findChild<QToolButton *>(QStringLiteral("sectionToggleButton"));
    QVERIFY(button != nullptr);
    QCOMPARE(button->minimumHeight(), style.headerHeight);
    QVERIFY(!button->isChecked());
}

void TestSection::setters_updateRuntimeConfiguration() {
    Section section(QStringLiteral("Runtime"), 100);

    auto *layout = new QVBoxLayout();
    layout->addWidget(new QLabel(QStringLiteral("Line 1")));
    layout->addWidget(new QLabel(QStringLiteral("Line 2")));
    section.setContentLayout(*layout);

    section.setAnimationDuration(250);
    section.setHeaderBackgroundColor(QColor("#111827"));
    section.setHeaderTextColor(QColor("#F3F4F6"));
    section.setContentTextColor(QColor("#9CA3AF"));
    section.setBorderColor(QColor("#374151"));
    section.setBorderWidth(3);
    section.setBorderRadius(10);
    section.setSectionSpacing(9);
    section.setContentMargins(QMargins(1, 2, 3, 4));
    section.setContentSpacing(7);
    section.setHeaderHeight(42);
    section.setIconColor(QColor("#F59E0B"));
    section.setIconSize(16);
    section.setIconStyle(Section::IconStyle::Chevron);
    section.setHoverEffectEnabled(true);
    section.setIconAnimationEnabled(true);
    section.setShadowBlurRadius(26);

    QFont headerFont(QStringLiteral("Sans"), 12, QFont::DemiBold);
    QFont contentFont(QStringLiteral("Sans"), 10, QFont::Medium);
    section.setHeaderFont(headerFont);
    section.setContentFont(contentFont);

    section.setShadowEnabled(true);
    QVERIFY(section.graphicsEffect() != nullptr);
    section.setShadowEnabled(false);
    QVERIFY(section.graphicsEffect() == nullptr);

    QCOMPARE(section.getAnimationDuration(), 250);
    QCOMPARE(section.getHeaderBackgroundColor(), QColor("#111827"));
    QCOMPARE(section.getHeaderTextColor(), QColor("#F3F4F6"));
    QCOMPARE(section.getContentTextColor(), QColor("#9CA3AF"));
    QCOMPARE(section.getBorderColor(), QColor("#374151"));
    QCOMPARE(section.getBorderWidth(), 3);
    QCOMPARE(section.getBorderRadius(), 10);
    QCOMPARE(section.getSectionSpacing(), 9);
    QCOMPARE(section.getContentMargins(), QMargins(1, 2, 3, 4));
    QCOMPARE(section.getContentSpacing(), 7);
    QCOMPARE(section.getHeaderHeight(), 42);
    QCOMPARE(section.getIconColor(), QColor("#F59E0B"));
    QCOMPARE(section.getIconSize(), 16);
    QCOMPARE(section.getIconStyle(), Section::IconStyle::Chevron);
    QVERIFY(section.isHoverEffectEnabled());
    QVERIFY(section.isIconAnimationEnabled());
    QCOMPARE(section.getShadowBlurRadius(), 26);
    QCOMPARE(section.getHeaderFont(), headerFont);
    QCOMPARE(section.getContentFont(), contentFont);

    QCOMPARE(layout->contentsMargins(), QMargins(1, 2, 3, 4));
    QCOMPARE(layout->spacing(), 7);

    auto *contentWidget = section.findChild<QWidget *>(QStringLiteral("sectionContentWidget"));
    QVERIFY(contentWidget != nullptr);
    QVERIFY(contentWidget->styleSheet().contains(QStringLiteral("ff9ca3af")));
}

QTEST_MAIN(TestSection)
#include "TestSection.moc"
