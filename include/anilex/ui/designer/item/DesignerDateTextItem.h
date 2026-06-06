#pragma once
#include "anilex/ui/interfaces/AbstractDesignerItem.h"


class DesignerDateTextItem : public AbstractDesignerItem {
  Q_OBJECT

  Q_PROPERTY(QString dateFormat READ dateFormat WRITE setDateFormat DESIGNABLE true);
  Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor DESIGNABLE true);
  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor DESIGNABLE true)
  Q_PROPERTY(QString stylesheet READ stylesheet WRITE setStylesheet DESIGNABLE false)
  Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment DESIGNABLE true)
  Q_PROPERTY(bool bold READ bold WRITE setBold DESIGNABLE true)
  Q_PROPERTY(int margin READ margin WRITE setMargin DESIGNABLE true)
  Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize DESIGNABLE true)

  Q_PROPERTY(int zValue READ zValue WRITE setZValue DESIGNABLE true)
public:
  DesignerDateTextItem();

  QString dateFormat() const;
  void setDateFormat(const QString& dateFormat);

  QColor textColor() const;
  void setTextColor(const QColor& textColor);

  QColor backgroundColor() const;
  void setBackgroundColor(const QColor& backgroundColor);

  QString stylesheet() const;
  void setStylesheet(const QString& stylesheet);

  Qt::Alignment alignment() const;
  void setAlignment(const Qt::Alignment& textAlignment);

  bool bold() const;
  void setBold(bool bold);

  int margin() const;
  void setMargin(int margin);

  int fontSize() const;
  void setFontSize(int fontSize);

  int zValue() const;
  void setZValue(int zValue);

  void fromJson(const QJsonObject &json) override;
  QJsonObject toJson() const override;
private:
  QString m_dateFormat;
};


