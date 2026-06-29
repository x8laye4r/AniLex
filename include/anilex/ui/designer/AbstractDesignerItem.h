#pragma once
#include <QWidget>

class AbstractDesignerItem : public QWidget {
public:
  explicit AbstractDesignerItem(QWidget *parent = nullptr);
  ~AbstractDesignerItem() override;

  virtual QJsonObject toJson() = 0;
  virtual void fromJson(const QJsonObject& jsonObject) = 0;

signals:

};
