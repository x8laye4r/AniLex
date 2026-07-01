#include "anilex/ui/designer/DesignerCanvas.h"

#include <QMimeData>
#include <QDropEvent>
#include <QJsonDocument>
#include <QJsonObject>

#include "anilex/ui/designer/DesignerItemFactory.h"
#include "anilex/utils/EnumConverter.h"

namespace DesignerHelpers = anilex_designer_helpers;

DesignerCanvas::DesignerCanvas(QWidget *parent)
  : QWidget(parent) {
  this->setAcceptDrops(true);
}

void DesignerCanvas::mousePressEvent(QMouseEvent *event) {
  QWidget::mousePressEvent(event);
}

void DesignerCanvas::mouseReleaseEvent(QMouseEvent *event) {
  QWidget::mouseReleaseEvent(event);
}

void DesignerCanvas::mouseMoveEvent(QMouseEvent *event) {
  QWidget::mouseMoveEvent(event);
}

void DesignerCanvas::dropEvent(QDropEvent *event) {
  const QMimeData *mimeData = event->mimeData();
  if (mimeData->hasFormat("anilex/designer-data")) {
    // read and validate JSON of the mime data
    QByteArray data = mimeData->data("anilex/designer-data");
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
      qInfo() << Q_FUNC_INFO << parseError.errorString();
      return;
    }
    QJsonObject json = doc.object();

    // get the type and the corresponding item from the factory
    const QString type = json["type"].toString("NONE");
    std::unique_ptr<IDesignerItem> item = DesignerItemFactory::instance().getItem(type, this);
    if (item == nullptr) {
      qInfo() << Q_FUNC_INFO << "Type is not available" << type;
      return;
    }

    IDesignerItem *designerItem = item.release();

    // set needed values from the JSON object
    designerItem->setType(EnumConverter::convertTo<DesignerHelpers::DesignerHelpers>(type)); // set to the correct type
    designerItem->installEventFilters_(this);
    designerItem->setGraphqlQuery(json.value("graphQL").toString(""));
    designerItem->setGraphqlSource(json.value("dataSource").toString(""));

  }
  QWidget::dropEvent(event);
}

void DesignerCanvas::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasFormat("anilex/designer-data")) [[likely]] {
    event->acceptProposedAction();
  } else [[unlikely]] {
    event->ignore();
  }
  QWidget::dragEnterEvent(event);
}

void DesignerCanvas::dragMoveEvent(QDragMoveEvent *event) {
  event->acceptProposedAction();
  QWidget::dragMoveEvent(event);
}

void DesignerCanvas::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
}
