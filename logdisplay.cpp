#include <QCoreApplication>
#include <QPlainTextEdit>
#include <QScrollBar>
#include "logdisplay.h"

void LogDisplay::append(const QString& text = "") {
  if (this) {
    this->appendPlainText(text);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());
  }
  
}
