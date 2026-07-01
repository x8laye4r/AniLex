#include "anilex/ui/designer/Designer.h"

#include "anilex/ui/designer/DesignerCanvas.h"
#include "anilex/ui/designer/DesignerWidgetsList.h"

Designer::Designer(QWidget *parent)
  : QWidget(parent) {
  this->setWindowTitle(tr("Designer"));
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setupUI();
}

void Designer::setupUI() {
  m_layout = new QHBoxLayout(this);

  auto *widgetsList = new DesignerWidgetsList(this);
  auto *canvas = new DesignerCanvas(this);

  m_layout->addWidget(widgetsList, 0);
  m_layout->addWidget(canvas, 1);
}