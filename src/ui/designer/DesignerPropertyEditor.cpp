#include "anilex/ui/designer/DesignerPropertyEditor.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QColorDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

/*
 * TODO: fix the issue with the new properties of the base class AbstractDesignerItem
 */

DesignerPropertyEditor::DesignerPropertyEditor(QWidget *parent)
  : QScrollArea(parent) {

  this->setWidgetResizable(true);

  QWidget *container = new QWidget(this);
  m_scrollLayout = new QVBoxLayout(container);
  m_scrollLayout->setContentsMargins(5, 5, 5, 5);
  m_scrollLayout->setSpacing(5);

  m_scrollLayout->addStretch();

  this->setWidget(container);

  this->initRegistry();
}

void DesignerPropertyEditor::clearEditor(QLayout *layoutToClear) {
  if (!layoutToClear) {
    layoutToClear = m_scrollLayout;
  }

  QLayoutItem* item;

  while ((item = m_scrollLayout->takeAt(0)) != nullptr) {
    if (QWidget* widget = item->widget()) {
      widget->hide();
      widget->deleteLater();
    }
    else if (QLayout* childLayout = item->layout()) {
      clearEditor(childLayout);
      childLayout->deleteLater();
    }
    delete item;
  }
}

static void getProperties(const QMetaObject *metaObject, QList<QMetaProperty> &properties) {
  const int property_count = metaObject->propertyCount();
  const int property_offset = metaObject->propertyOffset();

  for (int i = property_offset; i < property_count; ++i) {
    QMetaProperty property = metaObject->property(i);
    if (property.isWritable()) {
      properties.append(property);
    }
  }
}

void DesignerPropertyEditor::widgetChanged(AbstractDesignerItem *item) {
  if (item == nullptr) {
    return;
  }
  clearEditor();

  m_selectedItem = item;

  const QMetaObject *childClass = item->metaObject();
  const QMetaObject *parentClass = childClass->superClass();

  QList<QMetaProperty> writeableProperties;
  getProperties(childClass, writeableProperties);
  getProperties(parentClass, writeableProperties);

  this->createEditorWidget(writeableProperties);
}

void DesignerPropertyEditor::initRegistry() {
  m_editorRegistries[QMetaType::QString] = [](QWidget* parent, const QVariant& val, auto setter) {
    auto* edit = new QLineEdit(parent);
    edit->setText(val.toString());
    QObject::connect(edit, &QLineEdit::textChanged, setter);

    return edit;
  };

  m_editorRegistries[QMetaType::Int] = [](QWidget* parent, const QVariant& val, auto setter) {
    auto* spin = new QSpinBox(parent);
    spin->setRange(-9999, 9999);
    spin->setValue(val.toInt());
    QObject::connect(spin, &QSpinBox::valueChanged, [setter](const int i) { setter(i); });
    return spin;
  };

  m_editorRegistries[QMetaType::Bool] = [](QWidget* parent, const QVariant& val, auto setter) {
    auto* check = new QCheckBox(parent);
    check->setChecked(val.toBool());
    QObject::connect(check, &QCheckBox::toggled, [setter](const bool b) { setter(b); });
    return check;
  };

  m_editorRegistries[QMetaType::QColor] = [](QWidget* parent, const QVariant& val, auto setter) {
    auto* button = new QPushButton(parent);
    button->setText(QObject::tr("Choose Color"));
    QObject::connect(button, &QPushButton::clicked, [parent, val, setter] {
      QColorDialog *colorDialog = new QColorDialog(parent);
      colorDialog->setCurrentColor(val.value<QColor>());
      colorDialog->show();
      QObject::connect(colorDialog, &QColorDialog::currentColorChanged, [setter](const QColor& color) {
        setter(color);
      });
    });
    return button;
  };
}

QWidget *DesignerPropertyEditor::createEnumCreator(const QMetaProperty &property, const QVariant &value, auto setter) {
  QMetaEnum metaEnum = property.enumerator();

  QComboBox *comboBox = new QComboBox(this);
  const int count = metaEnum.keyCount();

  for (int i = 0; i < count; i++) {
    comboBox->addItem(QString::fromUtf8(metaEnum.key(i)), metaEnum.value(i));
  }

  int currentIndex = value.toInt();
  int idx = comboBox->findData(currentIndex);
  if (idx != -1) comboBox->setCurrentIndex(idx);

  connect(comboBox, &QComboBox::currentIndexChanged, [comboBox, setter](const int index) {
        setter(comboBox->itemData(index).toInt());
  });

  return comboBox;
}

void DesignerPropertyEditor::createEditorWidget(QList<QMetaProperty> &properties) {
  for (const QMetaProperty &property : properties) {
    QString propName = property.name();
    QVariant propValue = m_selectedItem->property(propName.toUtf8().constData());

    auto setter = [this, propName](const QVariant &newValue) {
      if (m_selectedItem) {
        m_selectedItem->setProperty(propName.toUtf8().constData(), newValue);
      }
    };

    QWidget *newEditorWidget = nullptr;

    if (property.isEnumType()) {
      newEditorWidget = createEnumCreator(property, propValue, setter);
    } else {
      int typeId = property.typeId();
      if (m_editorRegistries.contains(typeId)) {
        newEditorWidget = m_editorRegistries[typeId](this, propValue, setter);
      }
    }

    if (newEditorWidget) {
      QHBoxLayout *layout = new QHBoxLayout;

      layout->addWidget(new QLabel(tr("%1:").arg(propName)));
      layout->addWidget(newEditorWidget);

      m_scrollLayout->addLayout(layout);
    }
  }
  m_scrollLayout->addStretch(1);
}
