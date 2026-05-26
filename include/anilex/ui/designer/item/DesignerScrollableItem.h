#pragma once
#include "anilex/ui/interfaces/AbstractDesignerItem.h"

// some macros to for not writing as much
#define HORIZONTAL_SCROLLBAR_ON(scrollbar) scrollbar->horizontalScrollBarPolicy() == Qt::ScrollBarAsNeeded \
&& scrollArea->verticalScrollBarPolicy() == Qt::ScrollBarAlwaysOff
#define VERTICAL_SCROLLBAR_ON(scrollbar) scrollbar->horizontalScrollBarPolicy() == Qt::ScrollBarAlwaysOff \
&& scrollArea->verticalScrollBarPolicy() == Qt::ScrollBarAsNeeded

class DesignerScrollableItem : public AbstractDesignerItem {
  Q_OBJECT

  // scrollArea properties
  Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation);
  Q_PROPERTY(bool widgetResizable READ widgetResizable WRITE setWidgetResizable);
  Q_PROPERTY(bool showScrollBar READ showScrollBar WRITE setShowScrollBar);
  Q_PROPERTY(QString ScrollAreaStylesheet READ stylesheet WRITE setStylesheet)

  // properties scrollArea items
  Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
  Q_PROPERTY(QString itemStylesheet READ itemStylesheet WRITE setItemStylesheet)
  Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
  Q_PROPERTY(int margin READ margin WRITE setMargin)
  Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)

  Q_PROPERTY(int zValue READ zValue WRITE setZValue)
public:
  DesignerScrollableItem();

  Qt::Orientation orientation() const;
  void setOrientation(Qt::Orientation orientation);

  bool widgetResizable() const;
  void setWidgetResizable(bool resizable);

  bool showScrollBar() const;
  void setShowScrollBar(bool show);

  QString stylesheet() const;
  void setStylesheet(const QString &stylesheet);

  QColor textColor() const;
  void setTextColor(QColor color);

  QColor backgroundColor() const;
  void setBackgroundColor(QColor color);

  QString itemStylesheet() const;
  void setItemStylesheet(const QString &stylesheet);

  Qt::Alignment alignment() const;
  void setAlignment(Qt::Alignment alignment);

  int margin() const;
  void setMargin(int margin);

  int fontSize() const;
  void setFontSize(int size);

  void fromJson(const QJsonObject &json) override;
  QJsonObject toJson() const override;

private:
  void updateItemsLayout();
  void updateItemStyle();

  bool m_showScrollBar = true;
  QColor m_textColor = Qt::black;
  QColor m_textBackgroundColor = Qt::transparent;
  QString m_itemStylesheet = "";
  Qt::Alignment m_alignment = Qt::AlignCenter;
  int m_margin = 0;
  int m_fontSize = 0;
};
