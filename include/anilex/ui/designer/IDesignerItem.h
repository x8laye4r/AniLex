#pragma once
#include <QWidget>
#include "anilex/ui/designer/DesignerHelpers.h"

namespace DesignerTypes = anilex_designer_helpers;

class IDesignerItem {
public:
  explicit IDesignerItem() = default;
  virtual ~IDesignerItem() = default;

  virtual QJsonObject toJson() = 0;
  virtual void fromJson(const QJsonObject& jsonObject) = 0;
  virtual void installEventFilters_(QObject* eventFilterObject) = 0;

  virtual DesignerTypes::DesignerHelpers type() const = 0;
  virtual void setType(DesignerTypes::DesignerHelpers type) = 0;

  virtual QString graphqlSource() const = 0;
  virtual QString setGraphqlSource(const QString &source) = 0;

  virtual QString graphqlQuery() const = 0;
  virtual QString setGraphqlQuery(const QString &query) = 0;

  virtual bool isSelected() const = 0;
  virtual void setSelected(bool selected) = 0;

protected:
  /// GraphQL source where the graphql-builder puts it later. @note should be not empty and not edited after first set
  QString m_graphqlSource = "";
  /// GraphQL query what the graphql-builder puts later. @note should be not empty and not edited after first set
  QString m_graphqlQuery = "";
  /// type of the widget which gets used
  DesignerTypes::DesignerHelpers m_type = DesignerTypes::DesignerHelpers::NONE;
  /// the state in which it is (selected or not selected)
  bool m_isSelected = false; // defaults to be not selected
};
