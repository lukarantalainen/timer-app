#include <QCoreApplication>
#include <QPlainTextEdit>
#include <QScrollBar>
#include "logdisplay.h"
#include "input.h"

void LogDisplay::append(const KeyEvent event) {
  if (this) {
    this->appendPlainText(event.key_name);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());
  }
  
}
