#include "anilex/ui/designer/Designer.h"

#include "anilex/ui/designer/DesignerCanvas.h"
#include "anilex/ui/designer/DesignerWidgetsList.h"

Designer::Designer(QWidget *parent)
  : QWidget(parent) {
  this->setWindowTitle(tr("Designer"));
  this->setObjectName("designer");
  this->setAttribute(Qt::WA_DeleteOnClose);
  this->setupUI();
}

void Designer::setupUI() {
  m_layout = new QHBoxLayout(this);

  auto *widgetsList = new DesignerWidgetsList(this);

  auto *canvasLayout = new QVBoxLayout();
  canvasLayout->setContentsMargins(5, 5, 5, 5);

  canvasLayout->addStretch();
  auto *canvas = new DesignerCanvas(this);
  canvas->setMinimumSize(300, 200);
  canvasLayout->addWidget(canvas);
  canvasLayout->addStretch();

  m_layout->addWidget(widgetsList, 0);
  m_layout->addLayout(canvasLayout);
}