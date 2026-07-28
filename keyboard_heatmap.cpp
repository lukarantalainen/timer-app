#include "keyboard_heatmap.h"

#include <QGridLayout>
#include <QBoxLayout>
#include <QSpacerItem>
#include <list>
#include <string>
#include <iostream>
#include <QKeyEvent>
#include <QKeySequence>
#include <QString>

#include "keyboard_key.h"

KeyboardHeatmap::KeyboardHeatmap(KeyboardLayout layout, KeyboardSize size)
    : m_key_layout{layout}, m_size{size} {
  m_layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);

  createKeys();
  setStyleSheet("background-color: #222;");
}

void KeyboardHeatmap::keyDown(const KeyEvent& event) {
  std::string key_string = event.key_name.substr(4);
  QString qkeystr = QString::fromStdString(key_string);
  qDebug() << qkeystr;
  QKeySequence key = QKeySequence(qkeystr);
  qDebug() << key[0].key();

  KeyboardKey* key_object = m_keys[key[0].key()];
  if (key_object) {
    key_object->setStyle("color: red;");
  } 
}

void KeyboardHeatmap::keyUp(const KeyEvent& event) {
  std::string key_string = event.key_name.substr(4);
  QString qkeystr = QString::fromStdString(key_string);
  QKeySequence key = QKeySequence(qkeystr);

  KeyboardKey* key_object = m_keys[key[0].key()];
  if (key_object) {
    key_object->setDefaultStyle();
  } 
}

void KeyboardHeatmap::createKeys() {
  top_row = {
    new KeyboardKey(this, Qt::Key_Q),
    new KeyboardKey(this, Qt::Key_W),
    new KeyboardKey(this, Qt::Key_E),
    new KeyboardKey(this, Qt::Key_R),
    new KeyboardKey(this, Qt::Key_T),
    new KeyboardKey(this, Qt::Key_Y),
    new KeyboardKey(this, Qt::Key_U),
    new KeyboardKey(this, Qt::Key_I),
    new KeyboardKey(this, Qt::Key_O),
    new KeyboardKey(this, Qt::Key_P),
  };

  home_row = {
    new KeyboardKey(this, Qt::Key_A),
    new KeyboardKey(this, Qt::Key_S),
    new KeyboardKey(this, Qt::Key_D),
    new KeyboardKey(this, Qt::Key_F),
    new KeyboardKey(this, Qt::Key_G),
    new KeyboardKey(this, Qt::Key_H),
    new KeyboardKey(this, Qt::Key_J),
    new KeyboardKey(this, Qt::Key_K),
    new KeyboardKey(this, Qt::Key_L),
  };

  bottom_row =  {
    new KeyboardKey(this, Qt::Key_Z),
    new KeyboardKey(this, Qt::Key_X),
    new KeyboardKey(this, Qt::Key_C),
    new KeyboardKey(this, Qt::Key_V),
    new KeyboardKey(this, Qt::Key_B),
    new KeyboardKey(this, Qt::Key_N),
    new KeyboardKey(this, Qt::Key_M),
  };

  QBoxLayout* top_row_layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
  top_row_layout->setSpacing(4);
  int col{};
  for (auto it{top_row.begin()}; it != top_row.end(); ++it) {
    m_keys.insert({(*it)->m_key, *it});
    top_row_layout->addWidget(*it);
    ++col;
  }
  top_row_layout->addStretch();
  m_layout->addLayout(top_row_layout);

  QBoxLayout* home_row_layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
  home_row_layout->setSpacing(4);
  col = 0;
  for (auto it{home_row.begin()}; it != home_row.end(); ++it) {
    m_keys.insert({(*it)->m_key, *it});
    home_row_layout->addWidget(*it);
    ++col;
  }
  home_row_layout->addStretch();
  m_layout->addLayout(home_row_layout);

  QBoxLayout* bottom_row_layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
  bottom_row_layout->setSpacing(4);
  col = 0;
  for (auto it{bottom_row.begin()}; it != bottom_row.end(); ++it) {
    m_keys.insert({(*it)->m_key, *it});
    bottom_row_layout->addWidget(*it);
    ++col;
  }
  bottom_row_layout->addStretch();
  m_layout->addLayout(bottom_row_layout);
}
