#pragma once
#include <QLabel>
#include "anilex/ui/designer/IDesignerItem.h"

class DesignerTextItem : public QLabel, public IDesignerItem {
  Q_OBJECT
  Q_INTERFACES(IDesignerItem)

  Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor);
  Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor);
  Q_PROPERTY(Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment);
  Q_PROPERTY(bool bold READ bold WRITE setBold);
  Q_PROPERTY(qint32 fontSize READ fontSize WRITE setFontSize);
public:
  explicit DesignerTextItem(QWidget *parent = nullptr);
  QWidget *asWidget() override;

  void fromJson(const QJsonObject &jsonObject) override;
  QJsonObject toJson() override;
  void installEventFilters_(QObject *watched) override;

  QString graphqlQuery() const override;
  void setGraphqlQuery(const QString &query) override;

  QString graphqlSource() const override;
  void setGraphqlSource(const QString &source) override;

  bool isSelected() const override;
  void setSelected(bool selected) override;

  void setType(DesignerTypes::DesignerHelpers type) override;
  DesignerTypes::DesignerHelpers type() const override;

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

  ResizeHandle resizeHandleAt(const QPoint &localPos) const override;
  void setDraggingCursor(bool dragging) override;

signals:
  void propertyChanged(const QString &propertyName, const QVariant &newValue /* maybe add const QVariant &oldValue */);

protected:
  void paintEvent(QPaintEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

private:
  /// geometry of a single resize handle in local coordinates (shared by painting and hit-testing)
  QRect handleRect(ResizeHandle handle) const;
  /// pick the cursor shape matching a resize handle (open hand when over none)
  static Qt::CursorShape cursorForHandle(ResizeHandle handle);
  /// apply the hover cursor for a local position (resize cursor near a handle, open hand otherwise)
  void applyHoverCursor(const QPoint &localPos);

  static constexpr int K_HANDLE_SIZE = 8;
  static constexpr int MARGIN = 6;

  QColor m_backgroundColor = Qt::transparent;
  QColor m_textColor = Qt::black;
  Qt::Alignment m_textAlignment = Qt::AlignCenter;

  qint32 m_fontSize = 12;

  bool m_bold = false;
  bool m_isDragging = false;
};
