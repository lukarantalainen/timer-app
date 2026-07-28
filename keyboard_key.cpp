#include "keyboard_key.h"

#include <QChar>
#include <QKeyEvent>
#include <QLabel>
#include <QString>
#include <QTimer>
#include <QPalette>

KeyboardKey::KeyboardKey(QWidget* parent, Qt::Key key)
    : m_key{key}, QWidget(parent) {
  setFixedSize(50, 50);

  m_label = new QLabel(QtKeyToChar(m_key), this);
  m_label->setAlignment(Qt::AlignCenter);
  m_label->setFixedSize(50, 50);
  //m_label->setStyleSheet(m_default_style);
  m_label->show();
}

void KeyboardKey::setStyle(const QString& style) {
  //m_label->setStyleSheet(style);
}

void KeyboardKey::setDefaultStyle() {
  //m_label->setStyleSheet(m_default_style+m_background_color);
}

QString QtKeyToChar(Qt::Key key) {
  if (key >= Qt::Key_Space && key <= Qt::Key_AsciiTilde) {
    char key_char = static_cast<char>(key);
    return QString(key_char);
  }
  return "";
}

QColor operator*(QColor color, double x) {
  QRgb r = color.red()*x;
  QRgb g = color.green()*x;
  QRgb b = color.blue()*x;
  return QColor{QColor::fromRgb(r, g, b)};
}

QColor operator*(double x, QColor color) {
  QRgb r = color.red()*x;
  QRgb g = color.green()*x;
  QRgb b = color.blue()*x;
  return QColor{QColor::fromRgb(r, g, b)};
}

QColor operator+(QColor color1, QColor color2) {
  QRgb r = color1.red()+color2.red();
  QRgb g = color1.green()+color2.green();
  QRgb b = color1.blue()+color2.blue();
  return QColor{QColor::fromRgb(r, g, b)};
}

QColor operator-(QColor color1, QColor color2) {
  QRgb r = color1.red()+-color2.red();
  QRgb g = color1.green()-color2.green();
  QRgb b = color1.blue()-color2.blue();
  return QColor{QColor::fromRgb(r, g, b)};
}

void KeyboardKey::updateColor(double p) {
  QColor color;
  if (p < 0.5) {
    color = (color_b * p * 2.0) + color_a * (0.5 - p) * 2.0;
  } else {
    color = color_c * (p - 0.5) * 2.0 + color_b * (1.0 - p) * 2.0;
  }

  QPalette pal = m_label->palette();
  pal.setColor(QPalette::Window, color);
  m_label->setAutoFillBackground(true);
  m_label->setPalette(pal);

  
}
