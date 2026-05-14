#include "anilex/ui/designer/Designer.h"

#include "anilex/ui/designer/DesignerView.h"

Designer::Designer(QWidget *parent)
  : QFrame(parent) {
  m_designerView = new DesignerView(this);
  m_widgetList = new DesignerWidgetsList(this);

  this->setFrameStyle(Plain);
  this->setWindowFlag(Qt::Tool);
  this->setupUi();
}

void Designer::setupUi() {
  this->setObjectName("designerFrame");

}

void Designer::openDesigner() {
}

void Designer::closeDesigner() {
}
