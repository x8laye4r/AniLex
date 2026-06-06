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
  Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation DESIGNABLE true);
  Q_PROPERTY(bool widgetResizable READ widgetResizable WRITE setWidgetResizable DESIGNABLE true);
  Q_PROPERTY(bool showScrollBar READ showScrollBar WRITE setShowScrollBar DESIGNABLE true);
  Q_PROPERTY(QString ScrollAreaStylesheet READ stylesheet WRITE setStylesheet DESIGNABLE false )

  // properties scrollArea items
  Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor DESIGNABLE true)
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor DESIGNABLE true)
  Q_PROPERTY(QString itemStylesheet READ itemStylesheet WRITE setItemStylesheet DESIGNABLE false)
  Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment DESIGNABLE true)
  Q_PROPERTY(int margin READ margin WRITE setMargin DESIGNABLE true)
  Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize DESIGNABLE true)

  Q_PROPERTY(int zValue READ zValue WRITE setZValue DESIGNABLE true)
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
