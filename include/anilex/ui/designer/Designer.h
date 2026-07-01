#pragma once
#include <QWidget>
#include <QHBoxLayout>

class Designer : public QWidget {
  Q_OBJECT
public:
  explicit Designer(QWidget *parent = nullptr);
  ~Designer() override = default;

private:
  void setupUI();

  QHBoxLayout *m_layout = nullptr;
};