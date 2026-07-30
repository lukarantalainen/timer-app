#pragma once 

#include <QPlainTextEdit>
#include <QObject>
#include "input.h"

class LogDisplay : public QPlainTextEdit {
  Q_OBJECT

  public:

  LogDisplay(QWidget* parent);

  public slots:
    void append(const KeyEvent event);

};

