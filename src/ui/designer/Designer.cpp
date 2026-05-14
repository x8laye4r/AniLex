#include "anilex/ui/designer/Designer.h"

#include "anilex/ui/designer/DesignerView.h"
#include "anilex/utils/Helper.h"

Designer::Designer(QWidget *parent)
  : QFrame(parent) {
  m_frameLayout = new QHBoxLayout(this);
  this->setLayout(m_frameLayout);

  m_designerView = new DesignerView(this);
  m_widgetList = new DesignerWidgetsList(this);

  m_viewLayout = new QVBoxLayout;

  QMap<QString, QList<QJsonObject>> widgetCategoriesList = getDesignerWidgetsCategorized();
  m_widgetList->addWidgetSections(widgetCategoriesList);

  this->setFrameStyle(Plain);
  this->setWindowFlag(Qt::Tool);
  this->setupUi();
}

void Designer::setupUi() {
  this->setObjectName("designerFrame");
  m_frameLayout->addWidget(m_widgetList);

  m_viewLayout->setContentsMargins(0, 0, 0, 0);
  m_viewLayout->addStretch(1);
  m_viewLayout->addWidget(m_designerView, 0, Qt::AlignHCenter);
  m_viewLayout->addStretch(1);

  m_frameLayout->addLayout(m_viewLayout);
}

void Designer::exportWidgetsAsJson() {
  QList<QGraphicsItem*> widgets = m_designerView->scene()->items();
}

void Designer::openDesigner() {

}

void Designer::closeDesigner() {

}
