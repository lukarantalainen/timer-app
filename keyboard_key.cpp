#include "keyboard_key.h"

#include <QChar>
#include <QKeyEvent>
#include <QLabel>
#include <QString>
#include <QTimer>

KeyboardKey::KeyboardKey(QWidget* parent, Qt::Key key)
    : m_key{key}, QWidget(parent) {
  setFixedSize(50, 50);

  m_label = new QLabel(QtKeyToChar(m_key), this);
  m_label->setAlignment(Qt::AlignCenter);
  m_label->setFixedSize(50, 50);
  m_label->setStyleSheet(m_default_style);
  m_label->show();
}

void KeyboardKey::setStyle(const QString& style) {
  m_label->setStyleSheet(style);
}

void KeyboardKey::setDefaultStyle() {
  m_label->setStyleSheet(m_default_style);
}

QString QtKeyToChar(Qt::Key key) {
  if (key >= Qt::Key_Space && key <= Qt::Key_AsciiTilde) {
    char key_char = static_cast<char>(key);
    return QString(key_char);
  }
  return "";
}
