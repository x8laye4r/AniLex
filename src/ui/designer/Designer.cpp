#include "anilex/ui/designer/Designer.h"
#include "anilex/ui/designer/DesignerView.h"
#include "anilex/ui/interfaces/AbstractDesignerItem.h"
#include "anilex/utils/AppPaths.h"
#include "anilex/utils/Helper.h"

#include <QJsonArray>
#include <QShortcut>
#include <QMenu>
#include <QApplication>

#include "anilex/core/MenuRClickFilter.h"

Designer::Designer(QWidget *parent)
  : QFrame(parent) {

  m_frameLayout = new QHBoxLayout(this);
  this->setLayout(m_frameLayout);

  m_viewLayout = new QVBoxLayout;
  m_listViewLayout = new QVBoxLayout;
  m_propertiesLayout = new QVBoxLayout;

  m_designerView = new DesignerView(this);
  m_widgetList = new DesignerWidgetsList(this);
  m_widgetListLabel = new QLabel(tr("Widgets"), this);
  m_exportCardButton = new QPushButton(tr("Export Card"), this);
  m_designerPropertyEditor = new DesignerPropertyEditor(this);

  QMap<QString, QList<QJsonObject>> widgetCategoriesList = getDesignerWidgetsCategorized();
  m_widgetList->addWidgetSections(widgetCategoriesList);

  this->setFrameStyle(Plain);
  // this->setWindowFlag(Qt::Tool); seems to make the context menu not render in the right place
  this->setupObjectNames();
  this->setupUi();
  this->setupActions();
  this->setupConnections();
}

void Designer::setupUi() const {
  m_listViewLayout->addWidget(m_widgetListLabel);
  m_listViewLayout->addWidget(m_widgetList);

  m_viewLayout->addStretch(1);
  m_viewLayout->addWidget(m_designerView);
  m_designerView->setContextMenuPolicy(Qt::CustomContextMenu);
  m_viewLayout->addStretch(1);

  m_propertiesLayout->addWidget(m_designerPropertyEditor);
  m_propertiesLayout->addWidget(m_exportCardButton);

  m_frameLayout->addLayout(m_listViewLayout);
  m_frameLayout->addLayout(m_viewLayout);
  m_frameLayout->addLayout(m_propertiesLayout);
}

void Designer::setupActions() {
  m_deleteWidgetShortcut = new QShortcut(QKeySequence::Delete, this);
}

void Designer::setupObjectNames() {
  this->setObjectName("designerFrame");
  m_widgetListLabel->setObjectName("widgetListLabel");
  m_exportCardButton->setObjectName("exportCardButton");
}

void Designer::setupConnections() {
  connect(m_exportCardButton, &QPushButton::clicked, this, [this] {
    this->exportWidgetsAsJson(DesignerType::DesignerCreatorItems::ANIME_GENERAL);
  });

  connect(m_designerView->scene(), &QGraphicsScene::selectionChanged, m_designerPropertyEditor, [this] {
    QList<QGraphicsItem *> items = m_designerView->scene()->selectedItems();

    if (items.isEmpty()) {
      m_designerPropertyEditor->widgetChanged(nullptr);
      return;
    }

    if (items.size() > 1) {
      qInfo() << "Multiple items and changing properties will be added later";
      m_designerPropertyEditor->widgetChanged(nullptr);
      return;
    }

    QList<QGraphicsItem*> temp = items.first()->childItems();

    auto *item = qgraphicsitem_cast<AbstractDesignerItem*>(temp.first());
    item ? m_designerPropertyEditor->widgetChanged(item) : m_designerPropertyEditor->widgetChanged(nullptr);
  });

  connect(m_deleteWidgetShortcut, &QShortcut::activated, this, &Designer::deleteSelectedWidgets);
  connect(m_designerView, &DesignerView::customContextMenuRequested, this, &Designer::showCustomContextMenu);
}

void Designer::deleteSelectedWidgets() const {
  auto selected = m_designerView->scene()->selectedItems();
  for (QGraphicsItem *item : selected) {
    if (item->parentItem() && selected.contains(item->parentItem())) {
      continue;
    }
    delete item;
  }
}

void Designer::showCustomContextMenu(const QPoint &pos) {
  QMenu contextMenu(m_designerView);
  contextMenu.setObjectName("contextMenu");

  QAction *deleteSelectedAction = new QAction(tr("Delete Selected"), &contextMenu);
  deleteSelectedAction->setObjectName("contextMenuSelection");

  QAction *clearAllAction = new QAction(tr("Clear All"), &contextMenu);
  clearAllAction->setObjectName("contextMenuSelection");

  connect(deleteSelectedAction, &QAction::triggered, this, &Designer::deleteSelectedWidgets);
  connect(clearAllAction, &QAction::triggered, this, &Designer::clearView);

  MenuRClickFilter *eventFilter = new MenuRClickFilter();
  contextMenu.installEventFilter(eventFilter);

  contextMenu.addAction(deleteSelectedAction);
  contextMenu.addAction(clearAllAction);
  contextMenu.exec(m_designerView->viewport()->mapToGlobal(pos));
}

void Designer::exportWidgetsAsJson(DesignerType::DesignerCreatorItems cardToExportFor) const {
  QJsonObject json;
  QList<QGraphicsItem*> widgets = m_designerView->scene()->items();
  QJsonArray arrayOfWidgets;

  for (QGraphicsItem *widget : widgets) {
    if (auto *item = dynamic_cast<AbstractDesignerItem *>(widget)) {
      arrayOfWidgets.append(item->toJson());
    }
  }

  QJsonDocument jsonDocument(arrayOfWidgets);

  QString path = AppPaths::appDataPath();
  path.append("/" + EnumConverter::toString(cardToExportFor) + ".json");

  QFile file(path);
  qInfo() << path;
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qWarning() << "Couldn't open file in Designer::exportWidgetsAsJson";
    qInfo() << file.errorString();
    return;
  }

  file.write(jsonDocument.toJson(QJsonDocument::Indented));
  file.close();
}

void Designer::openDesigner() {
  this->show();
}

void Designer::closeDesigner() {
  this->close();
}

void Designer::clearView() const {
  m_designerView->scene()->clear();
}
