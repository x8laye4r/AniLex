#pragma once
#include <QWidget>
#include "anilex/ui/designer/DesignerHelpers.h"

namespace DesignerTypes = anilex_designer_helpers;

class IDesignerItem {
public:
  /// the resize handles that can be hit-tested on the item where it is getting resized
  enum class ResizeHandle {
    None,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
  };

  explicit IDesignerItem() = default;
  virtual ~IDesignerItem() = default;
  /// Every designer item is a QWidget; concrete classes return `this`. Never null.
  /// A QWidget is also a QObject, so this doubles as the item's QObject handle.
  virtual QWidget *asWidget() = 0;

  virtual QJsonObject toJson() = 0;
  virtual void fromJson(const QJsonObject& jsonObject) = 0;
  virtual void installEventFilters_(QObject* eventFilterObject) = 0;

  /// hit-test the resize handles in the item's own coordinate space
  virtual ResizeHandle resizeHandleAt(const QPoint& localPos) const = 0;
  /// let the item show/clear its drag (grabbing) cursor; driven by the canvas
  virtual void setDraggingCursor(bool dragging) = 0;

  virtual DesignerTypes::DesignerHelpers type() const = 0;
  virtual void setType(DesignerTypes::DesignerHelpers type) = 0;

  virtual QString graphqlSource() const = 0;
  virtual void setGraphqlSource(const QString &source) = 0;

  virtual QString graphqlQuery() const = 0;
  virtual void setGraphqlQuery(const QString &query) = 0;

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

Q_DECLARE_INTERFACE(IDesignerItem, "net.x8laye4r.IDesignerItem")