#include "logdisplay.h"

#include <QCoreApplication>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <sstream>
#include <QString>
#include <QDateTime>

#include "keyevent.h"

Flags operator|(Flags lhs, Flags rhs) {
  return static_cast<Flags>(
    static_cast<std::underlying_type<Flags>::type>(lhs) | 
    static_cast<std::underlying_type<Flags>::type>(rhs)
  );
}

bool operator&(Flags lhs, Flags rhs) {
  return static_cast<bool>(
    static_cast<std::underlying_type<Flags>::type>(lhs) &
    static_cast<std::underlying_type<Flags>::type>(rhs)
  );
}


LogDisplay::LogDisplay(QWidget* parent) : QWidget(parent) {
  m_textedit = new QPlainTextEdit(this);
  m_textedit->setReadOnly(true);
}

void LogDisplay::print(const QString input, Flags flags) {
  QString text = input;
  if (flags & Flags::TIMESTAMP) {
    auto time = QDateTime::currentDateTime().toString("hh:mm:ss");
    text = time;
    text.append(" ");
    text.append(input);
  }
  m_textedit->appendPlainText(text);
  m_textedit->verticalScrollBar()->setValue(m_textedit->verticalScrollBar()->maximum());
}

void LogDisplay::append(const KeyEvent event) {
  if (this) {
    input_event input_data = event.input_data;

    std::ostringstream oss;
    oss << "time: " << input_data.time.tv_sec << " type: " << input_data.type
        << " code: " << input_data.code << " value: " << input_data.value
        << " key: " << event.key_name;

    m_textedit->appendPlainText(QString::fromStdString(oss.str()));
    m_textedit->verticalScrollBar()->setValue(
        m_textedit->verticalScrollBar()->maximum());
  }
}
