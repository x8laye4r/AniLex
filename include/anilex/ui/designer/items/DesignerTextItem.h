#pragma once
#include <QLabel>
#include "anilex/ui/designer/IDesignerItem.h"

class DesignerTextItem : public QLabel, public IDesignerItem {
  Q_OBJECT

  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor);
  Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor);
  Q_PROPERTY(Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment);
  Q_PROPERTY(bool bold READ bold WRITE setBold);
  Q_PROPERTY(qint32 fontSize READ fontSize WRITE setFontSize);
public:
  explicit DesignerTextItem(QWidget *parent = nullptr);

  void fromJson(const QJsonObject &jsonObject) override;
  QJsonObject toJson() override;
  void installEventFilters_(QObject *watched) override;

  QString graphqlQuery() const override;
  QString setGraphqlQuery(const QString &query) override;

  QString graphqlSource() const override;
  QString setGraphqlSource(const QString &source) override;

  bool isSelected() const override;
  void setSelected(bool selected) override;

  void setType(DesignerTypes::Types type) override;
  DesignerTypes::Types type() const override;

  QColor backgroundColor() const;
  void setBackgroundColor(const QColor& color);

  QColor textColor() const;
  void setTextColor(const QColor& color);

  Qt::Alignment textAlignment() const;
  void setTextAlignment(Qt::Alignment alignment);

  bool bold() const;
  void setBold(bool bold);

  qint32 fontSize() const;
  void setFontSize(qint32 fontSize);

signals:
  void propertyChanged(const QString &propertyName, const QVariant &newValue /* maybe add const QVariant &oldValue */);

private:
  QColor m_backgroundColor;
  QColor m_textColor;
  Qt::Alignment m_textAlignment;

  qint32 m_fontSize;

  bool m_bold;
};
