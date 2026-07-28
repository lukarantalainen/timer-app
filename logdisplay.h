#pragma once 

#include <QPlainTextEdit>
#include <QObject>
#include "input.h"

class LogDisplay : public QPlainTextEdit {
  Q_OBJECT

  public:

  LogDisplay(QWidget* parent) : QPlainTextEdit(parent) {

  }

  public slots:
    void append(const KeyEvent event);

};

