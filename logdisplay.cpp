#include <QCoreApplication>
#include <QPlainTextEdit>
#include <QScrollBar>
#include "logdisplay.h"
#include "input.h"
#include <sstream>

LogDisplay::LogDisplay(QWidget* parent) : QPlainTextEdit(parent) {
  setReadOnly(true);
}

void LogDisplay::append(const KeyEvent event) {
  if (this) {

    input_event input_data = event.input_data;
    
    std::ostringstream oss;
    oss << "time: " << input_data.time.tv_sec << " type: " << input_data.type
    << " code: " << input_data.code << " value: " << input_data.value
    << " key: " << event.key_name;

    this->appendPlainText(QString::fromStdString(oss.str()));
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());
  }
  
}
