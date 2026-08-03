#pragma once 

#include <QPlainTextEdit>
#include <QWidget>
#include <QObject>
#include "input.h"

class LogDisplay : public QWidget {
  Q_OBJECT

  public:

  LogDisplay(QWidget* parent);

  public slots:
    void append(const KeyEvent event);

  private:
  QPlainTextEdit* m_textedit;
};

