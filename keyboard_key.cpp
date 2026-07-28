#include "keyboard_key.h"
#include <QLabel>
#include <QString>
#include <QChar>
#include <QKeyEvent>

KeyboardKey::KeyboardKey(QWidget* parent, Qt::Key key) : m_key {key}, QWidget(parent) {
    setFixedSize(50, 50);
    
    m_label = new QLabel(QtKeyToChar(m_key), this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setFixedSize(50, 50);
    m_label->setStyleSheet("border: 1px solid #444; border-radius: 4px; background-color: #333; color: white; font-weight: bold;");
    m_label->show();
  }

void KeyboardKey::setStyle(const QString& style) {
  m_label->setStyleSheet(style);
}


QString QtKeyToChar(Qt::Key key) {
  if (key >= Qt::Key_Space && key <= Qt::Key_AsciiTilde) {
    char key_char = static_cast<char>(key);
    return QString(key_char);
  }
  return "";
}
