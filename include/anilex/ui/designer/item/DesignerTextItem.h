#pragma once
#include <QTextFormat>
#include "anilex/ui/interfaces/AbstractDesignerItem.h"

class DesignerTextItem : public AbstractDesignerItem {
  Q_OBJECT

  // All changeable properties for the user
  Q_PROPERTY(QString text READ text WRITE setText)
  Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
  Q_PROPERTY(QString stylesheet READ stylesheet WRITE setStylesheet)
  Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
  Q_PROPERTY(bool bold READ bold WRITE setBold)
  Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)
  Q_PROPERTY(int margin READ margin WRITE setMargin)
  Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)

  Q_PROPERTY(int zValue READ zValue WRITE setZValue)
public:
  DesignerTextItem();

  QString text() const;
  void setText(const QString& text);

  QColor textColor() const;
  void setTextColor(const QColor& textColor);

  QColor backgroundColor() const;
  void setBackgroundColor(const QColor& backgroundColor);

  QString stylesheet() const;
  void setStylesheet(const QString& stylesheet);

  Qt::Alignment alignment() const;
  void setAlignment(Qt::Alignment alignment);

  bool bold() const;
  void setBold(bool bold);

  bool wordWrap() const;
  void setWordWrap(bool wordWrap);

  int margin() const;
  void setMargin(int margin);

  int fontSize() const;
  void setFontSize(int fontSize);

  int zValue() const;
  void setZValue(int zValue);

  void fromJson(const QJsonObject &json) override;
  QJsonObject toJson() const override;
};
