#include "keyboard_key.h"

#include <QChar>
#include <QKeyEvent>
#include <QLabel>
#include <QString>
#include <QTimer>
#include <QPalette>
#include <QGridLayout>

KeyboardKey::KeyboardKey(QWidget* parent, Qt::Key key)
    : m_key{key}, QWidget(parent) {
  setFixedSize(50, 50);

  setStyleSheet("background-color: #47db34; border-radius: 5px;");

  pal = palette();

  QGridLayout* layout = new QGridLayout(this);

  layout->setContentsMargins(4, 4, 4, 4); 
  layout->setSpacing(0);

  layout->setRowStretch(0, 1);
  layout->setRowStretch(2, 1);
  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(2, 1);

  m_label = new QLabel(QtKeyToChar(m_key), this);
  m_label->adjustSize();
  
  layout->addWidget(m_label, 1, 1, Qt::AlignCenter);

  m_label_palette = m_label->palette();
  m_label->setAutoFillBackground(true);
  m_label->setPalette(m_label_palette);

  m_count_label = new QLabel(QString::number(0), this);
  m_count_label->move(2, -8);
  m_count_label->setStyleSheet("background: transparent; color: black;"); 

  QFont font = m_count_label->font();
  font.setPointSize(8);
  m_count_label->setFont(font);
}

void KeyboardKey::setValue(int new_value) {
  m_count = new_value;
  m_count_label->setText(QString::number(m_count));
}


void KeyboardKey::increment() {
    ++m_count;
}

void KeyboardKey::keyDown() {
  increment();
  m_count_label->setText(QString::number(m_count));

  m_label_palette.setColor(QPalette::WindowText, Qt::white);
}

void KeyboardKey::keyUp() {
  m_label_palette.setColor(QPalette::WindowText, Qt::black);
  m_label->setPalette(m_label_palette);
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

  pal.setColor(QPalette::Window, color);
  auto style_sheet = QString("border-radius: 5px; background-color: %1;")
  .arg(color.name(QColor::HexArgb));
  setStyleSheet(style_sheet);
}
