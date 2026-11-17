#include "keyboard_heatmap.h"

#include <QBoxLayout>
#include <QGridLayout>
#include <QKeyEvent>
#include <QKeySequence>
#include <QPalette>
#include <QSpacerItem>
#include <QString>
#include <iostream>
#include <list>
#include <string>

#include "database.h"
#include "keyboard_key.h"
#include "keyevent.h"

KeyboardHeatmap::KeyboardHeatmap(KeyboardLayout layout, KeyboardSize size,
                                 QWidget* parent)
    : m_key_layout{layout}, m_size{size}, QWidget(parent) {
  m_layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);

  setAutoFillBackground(true);
  QPalette pal = this->palette();
  this->setPalette(pal);
  createKeys();
}

double getPercentage(int min, int max, int x) {
  int min_copy{min};
  min -= min_copy;
  max -= min_copy;
  x -= min_copy;

  return (double)x / max;
}

std::vector<std::pair<QString, int>> KeyboardHeatmap::getKeyData() {
  std::vector<std::pair<QString, int>> keys;
  for (auto pair : m_keys) {
    int count = (pair.second) ? pair.second->getCount() : 0;
    keys.push_back({pair.first, count});
  }
  return keys;
};

void KeyboardHeatmap::setValue(QString key, int value) {
  KeyboardKey* key_object = m_keys[key];
  if (key_object) {
    key_object->setValue(value);

    int count{key_object->getCount()};

    if (count < min_value) {
      min_value = count;
    } else if (count > max_value) {
      max_value = count;
    }

    double percentage = getPercentage(min_value, max_value, count);

    key_object->updateColor(percentage);
  }
}

void KeyboardHeatmap::keyDown(const KeyEvent& event) {
  std::string key_string = event.key_name.substr(4);
  QString qkeystr = QString::fromStdString(key_string);
  qDebug() << qkeystr;
  QKeySequence key = QKeySequence(qkeystr);

  KeyboardKey* key_object = m_keys[qkeystr];
  if (key_object) {
    key_object->keyDown();
    int count{key_object->getCount()};
    if (count < min_value) {
      min_value = count;
    } else if (count > max_value) {
      max_value = count;
    }

    double percentage = getPercentage(min_value, max_value, count);

    key_object->updateColor(percentage);
  }
}

void KeyboardHeatmap::keyUp(const KeyEvent& event) {
  QString key_string = QString::fromStdString(event.key_name.substr(4));
  std::string std_string = event.key_name.substr(4);
  QString qkeystr = QString::fromStdString(std_string);
  QKeySequence key = QKeySequence(qkeystr);

  // KeyboardKey* key_object = m_keys[key[0].key()];
  KeyboardKey* key_object = m_keys[key_string];
  if (key_object) {
    key_object->keyUp();
  }
}

QBoxLayout* KeyboardHeatmap::createLayout(std::list<KeyboardKey*> row) {
  int col{};
  QBoxLayout* layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
  layout->setSpacing(4);
  for (auto it{row.begin()}; it != row.end(); ++it) {
    QString key_name = (*it)->name.toUpper();
    // m_keys.insert({(*it)->m_key, *it});
    m_keys.insert({key_name, *it});
    qDebug() << "key name" << key_name;
    layout->addWidget(*it);
    ++col;
  }
  layout->addStretch();
  return layout;
}

void KeyboardHeatmap::createKeys() {
  row0 = {
      new KeyboardKey(this, Qt::Key_F1, "F1"),
      new KeyboardKey(this, Qt::Key_F2, "F2"),
      new KeyboardKey(this, Qt::Key_F3, "F3"),
      new KeyboardKey(this, Qt::Key_F4, "F4"),
      new KeyboardKey(this, Qt::Key_F5, "F5"),
      new KeyboardKey(this, Qt::Key_F6, "F6"),
      new KeyboardKey(this, Qt::Key_F7, "F7"),
      new KeyboardKey(this, Qt::Key_F8, "F8"),
      new KeyboardKey(this, Qt::Key_F9, "F9"),
      new KeyboardKey(this, Qt::Key_F10, "F10"),
      new KeyboardKey(this, Qt::Key_F11, "F11"),
      new KeyboardKey(this, Qt::Key_F12, "F12"),
      new KeyboardKey(this, Qt::Key_Print, "SysRq"),
      new KeyboardKey(this, Qt::Key_ScrollLock, "ScrollLock"),
      new KeyboardKey(this, Qt::Key_Pause, "Pause"),

  };

  row1 = {
      new KeyboardKey(this, Qt::Key_AsciiTilde, "Grave"),
      new KeyboardKey(this, Qt::Key_1, "1"),
      new KeyboardKey(this, Qt::Key_2, "2"),
      new KeyboardKey(this, Qt::Key_3, "3"),
      new KeyboardKey(this, Qt::Key_4, "4"),
      new KeyboardKey(this, Qt::Key_5, "5"),
      new KeyboardKey(this, Qt::Key_6, "6"),
      new KeyboardKey(this, Qt::Key_7, "7"),
      new KeyboardKey(this, Qt::Key_8, "8"),
      new KeyboardKey(this, Qt::Key_9, "9"),
      new KeyboardKey(this, Qt::Key_0, "0"),
      new KeyboardKey(this, Qt::Key_Equal, "Equal"),
      new KeyboardKey(this, Qt::Key_Backspace, "Backspace"),
      new KeyboardKey(this, Qt::Key_Insert, "Insert"),
      new KeyboardKey(this, Qt::Key_Home, "Home"),
      new KeyboardKey(this, Qt::Key_PageUp, "PageUp"),

  };

  row2 = {
      new KeyboardKey(this, Qt::Key_Tab, "Tab"),
      new KeyboardKey(this, Qt::Key_Q, "Q"),
      new KeyboardKey(this, Qt::Key_W, "W"),
      new KeyboardKey(this, Qt::Key_E, "E"),
      new KeyboardKey(this, Qt::Key_R, "R"),
      new KeyboardKey(this, Qt::Key_T, "T"),
      new KeyboardKey(this, Qt::Key_Y, "Y"),
      new KeyboardKey(this, Qt::Key_U, "U"),
      new KeyboardKey(this, Qt::Key_I, "I"),
      new KeyboardKey(this, Qt::Key_O, "O"),
      new KeyboardKey(this, Qt::Key_P, "P"),
      new KeyboardKey(this, Qt::Key_BraceLeft, "LeftBrace"),
      new KeyboardKey(this, Qt::Key_BraceRight, "Rightbrace"),
      new KeyboardKey(this, Qt::Key_Enter, "Enter"),
      new KeyboardKey(this, Qt::Key_Delete, "Delete"),
      new KeyboardKey(this, Qt::Key_End, "End"),
      new KeyboardKey(this, Qt::Key_PageDown, "PageDown"),

  };

  row3 = {
      new KeyboardKey(this, Qt::Key_CapsLock, "CapsLock"),
      new KeyboardKey(this, Qt::Key_A, "A"),
      new KeyboardKey(this, Qt::Key_S, "S"),
      new KeyboardKey(this, Qt::Key_D, "D"),
      new KeyboardKey(this, Qt::Key_F, "F"),
      new KeyboardKey(this, Qt::Key_G, "G"),
      new KeyboardKey(this, Qt::Key_H, "H"),
      new KeyboardKey(this, Qt::Key_J, "J"),
      new KeyboardKey(this, Qt::Key_K, "K"),
      new KeyboardKey(this, Qt::Key_L, "L"),
      new KeyboardKey(this, Qt::Key_Backslash, "Backslash"),

  };

  row4 = {
      new KeyboardKey(this, Qt::Key_Shift, "LeftShift"),
      new KeyboardKey(this, Qt::Key_Z, "Z"),
      new KeyboardKey(this, Qt::Key_X, "X"),
      new KeyboardKey(this, Qt::Key_C, "C"),
      new KeyboardKey(this, Qt::Key_V, "V"),
      new KeyboardKey(this, Qt::Key_B, "B"),
      new KeyboardKey(this, Qt::Key_N, "N"),
      new KeyboardKey(this, Qt::Key_M, "M"),
      new KeyboardKey(this, Qt::Key_Comma, "Comma"),
      new KeyboardKey(this, Qt::Key_Period, "Dot"),
      new KeyboardKey(this, Qt::Key_Period, "Slash"),
      new KeyboardKey(this, Qt::Key_Shift, "RightShift"),
      new KeyboardKey(this, Qt::Key_Left, "Up"),
  };

  row5 = {
      new KeyboardKey(this, Qt::Key_Control, "LeftCtrl"),
      new KeyboardKey(this, Qt::Key_Meta, "LeftMeta"),
      new KeyboardKey(this, Qt::Key_Alt, "LeftAlt"),
      new KeyboardKey(this, Qt::Key_Space, "Space"),
      new KeyboardKey(this, Qt::Key_AltGr, "RightAlt"),
      new KeyboardKey(this, Qt::Key_Menu, "Compose"),
      new KeyboardKey(this, Qt::Key_Control, "RightCtrl"),
      new KeyboardKey(this, Qt::Key_Left, "Left"),
      new KeyboardKey(this, Qt::Key_Down, "Down"),
      new KeyboardKey(this, Qt::Key_Right, "Right"),
  };

  auto row0_layout = createLayout(row0);
  m_layout->addLayout(row0_layout);

  auto row1_layout = createLayout(row1);
  m_layout->addLayout(row1_layout);

  auto row2_layout = createLayout(row2);
  m_layout->addLayout(row2_layout);

  auto row3_layout = createLayout(row3);
  m_layout->addLayout(row3_layout);

  auto row4_layout = createLayout(row4);
  m_layout->addLayout(row4_layout);

  auto row5_layout = createLayout(row5);
  m_layout->addLayout(row5_layout);
}
