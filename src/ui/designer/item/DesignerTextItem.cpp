#include "anilex/ui/designer/items/DesignerTextItem.h"
#include "anilex/ui/designer/DesignerItemFactory.h"
#include <QJsonObject>
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>

DesignerTextItem::DesignerTextItem(QWidget *parent)
  : QLabel(parent) {
  m_type = DesignerTypes::DesignerHelpers::TEXT;
  this->setText("Test Test");
  this->setContentsMargins(MARGIN, MARGIN, MARGIN, MARGIN); // make space for the outlines
}

QWidget * DesignerTextItem::asWidget() {
  return this;
}

void DesignerTextItem::fromJson(const QJsonObject &jsonObject) {
}

QJsonObject DesignerTextItem::toJson() {
  return {};
}

void DesignerTextItem::installEventFilters_(QObject *watched) {
  this->installEventFilter(watched);
}

QString DesignerTextItem::graphqlQuery() const {
  return m_graphqlQuery;
}

void DesignerTextItem::setGraphqlQuery(const QString &query) {
  m_graphqlQuery = query;
}

QString DesignerTextItem::graphqlSource() const {
  return m_graphqlSource;
}

void DesignerTextItem::setGraphqlSource(const QString &source) {
  m_graphqlSource = source;
}

bool DesignerTextItem::isSelected() const {
  return m_isSelected;
}

void DesignerTextItem::setSelected(const bool selected) {
  m_isSelected = selected;
}

void DesignerTextItem::setType(const DesignerTypes::DesignerHelpers type) {
  m_type = type;
}

DesignerTypes::DesignerHelpers DesignerTextItem::type() const {
  return m_type;
}

QColor DesignerTextItem::backgroundColor() const {
  return m_backgroundColor;
}

void DesignerTextItem::setBackgroundColor(const QColor &color) {
  if (m_backgroundColor != color) {
    m_backgroundColor = color;
    this->update();
    emit this->propertyChanged("backgroundColor", color);
  }
}

QColor DesignerTextItem::textColor() const {
  return m_textColor;
}

void DesignerTextItem::setTextColor(const QColor &color) {
  if (m_textColor != color) {
    m_textColor = color;
    this->update();
    emit this->propertyChanged("textColor", color);
  }
}

Qt::Alignment DesignerTextItem::textAlignment() const {
  return m_textAlignment;
}

void DesignerTextItem::setTextAlignment(const Qt::Alignment alignment) {
  if (m_textAlignment != alignment) {
    m_textAlignment = alignment;
    this->update();
    emit this->propertyChanged("textAlignment", QVariant::fromValue(alignment));
  }
}

bool DesignerTextItem::bold() const {
  return m_bold;
}

void DesignerTextItem::setBold(bool bold) {
  if (m_bold != bold) {
    m_bold = bold;
    this->update();
    emit this->propertyChanged("bold", bold);
  }
}

qint32 DesignerTextItem::fontSize() const {
  return m_fontSize;
}

void DesignerTextItem::setFontSize(qint32 fontSize) {
  if (m_fontSize != fontSize){
    m_fontSize = fontSize;
    this->update();
    emit this->propertyChanged("fontSize", fontSize);
  }
}

void DesignerTextItem::paintEvent(QPaintEvent *event) {
  QLabel::paintEvent(event); // paint the widget first

  QPainter painter(this);

  // draw outline if selected
  if (m_isSelected) {
    // draw outer rectangle
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::white, 2, Qt::DashLine));

    // push the rect inwards so it doesn't drow it clipped
    const QRect widgetRect = this->rect().adjusted(4, 4, -4, -4);
    painter.drawRect(widgetRect);

    // draw squares for resizing help
    painter.setPen(QPen(Qt::white, 2));
    painter.setBrush(QBrush(Qt::white));

    const QRect rectangles[] = {
      this->handleRect(ResizeHandle::TopLeft),
      this->handleRect(ResizeHandle::BottomLeft),
      this->handleRect(ResizeHandle::TopRight),
      this->handleRect(ResizeHandle::BottomRight),
    };

    painter.drawRects(rectangles, std::size(rectangles));
  }
}

/// get the rect for the resize handle
QRect DesignerTextItem::handleRect(const ResizeHandle handle) const {
  const QRect outline = this->rect().adjusted(4, 4, -4, -4);
  constexpr QPoint half(K_HANDLE_SIZE / 2, K_HANDLE_SIZE / 2);
  constexpr QSize size(K_HANDLE_SIZE, K_HANDLE_SIZE);

  switch (handle) {
    case ResizeHandle::TopLeft:     return {outline.topLeft() - half, size};
    case ResizeHandle::TopRight:    return {outline.topRight() - half, size};
    case ResizeHandle::BottomLeft:  return {outline.bottomLeft() - half, size};
    case ResizeHandle::BottomRight: return {outline.bottomRight() - half, size};
    default:                        return {};
  }
}

/// returns the handle where the resize is happening
IDesignerItem::ResizeHandle DesignerTextItem::resizeHandleAt(const QPoint &localPos) const {
  for (const ResizeHandle handle : {ResizeHandle::TopLeft, ResizeHandle::TopRight,
                                    ResizeHandle::BottomLeft, ResizeHandle::BottomRight}) {
    if (this->handleRect(handle).contains(localPos)) return handle;
  }
  return ResizeHandle::None;
}

/// get the correct cursor shape for resizing/dragging
Qt::CursorShape DesignerTextItem::cursorForHandle(const ResizeHandle handle) {
  switch (handle) {
    case ResizeHandle::TopLeft:
    case ResizeHandle::BottomRight:
      return Qt::SizeFDiagCursor;
    case ResizeHandle::TopRight:
    case ResizeHandle::BottomLeft:
      return Qt::SizeBDiagCursor;
    default:
      return Qt::OpenHandCursor; // general hover over the widget body
  }
}

/// set the correct cursor for the correct situation
void DesignerTextItem::applyHoverCursor(const QPoint &localPos) {
  if (m_isDragging) return;
  const ResizeHandle handle = m_isSelected ? resizeHandleAt(localPos) : ResizeHandle::None;
  this->setCursor(cursorForHandle(handle));
}

void DesignerTextItem::enterEvent(QEnterEvent *event) {
  this->applyHoverCursor(event->position().toPoint());
  QLabel::enterEvent(event);
}

void DesignerTextItem::leaveEvent(QEvent *event) {
  if (!m_isDragging) this->unsetCursor(); // reset the cursor to normal
  QLabel::leaveEvent(event);
}

void DesignerTextItem::mouseMoveEvent(QMouseEvent *event) {
  if (!(event->buttons() & Qt::LeftButton)) {
    this->applyHoverCursor(event->position().toPoint());
  }
  QLabel::mouseMoveEvent(event);
}

void DesignerTextItem::setDraggingCursor(const bool dragging) {
  m_isDragging = dragging;
  if (dragging) {
    this->setCursor(Qt::ClosedHandCursor); // grabbing while moving the item
  } else if (this->underMouse()) {
    this->applyHoverCursor(this->mapFromGlobal(QCursor::pos()));
  } else {
    this->unsetCursor();
  }
}


REGISTER_DESIGNER_ITEM(DesignerTextItem, "TEXT")
