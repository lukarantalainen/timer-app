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
    QString key_name = (*it)->label.toUpper();
    // m_keys.insert({(*it)->name, *it});
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
      new KeyboardKey(this, "F1", "F1"),   new KeyboardKey(this, "F2", "F2"),
      new KeyboardKey(this, "F3", "F3"),   new KeyboardKey(this, "F4", "F4"),
      new KeyboardKey(this, "F5", "F5"),   new KeyboardKey(this, "F6", "F6"),
      new KeyboardKey(this, "F7", "F7"),   new KeyboardKey(this, "F8", "F8"),
      new KeyboardKey(this, "F9", "F9"),   new KeyboardKey(this, "F10", "F10"),
      new KeyboardKey(this, "F11", "F11"), new KeyboardKey(this, "F12", "F12"),

  };

  row1 = {
      new KeyboardKey(this, "Grave", "§"),
      new KeyboardKey(this, "1", "1"),
      new KeyboardKey(this, "2", "2"),
      new KeyboardKey(this, "3", "3"),
      new KeyboardKey(this, "4", "4"),
      new KeyboardKey(this, "5", "5"),
      new KeyboardKey(this, "6", "6"),
      new KeyboardKey(this, "7", "7"),
      new KeyboardKey(this, "8", "8"),
      new KeyboardKey(this, "9", "9"),

      new KeyboardKey(this, "0", "0"),
      new KeyboardKey(this, "Equal", "="),
      new KeyboardKey(this, "Backspace", "Backspace"),
      new KeyboardKey(this, "Insert", "Ins"),
      new KeyboardKey(this, "Home", "Home"),
      new KeyboardKey(this, "PageUp", "PgUp"),

  };

  row2 = {
      new KeyboardKey(this, "Tab", "Tab"),
      new KeyboardKey(this, "Q", "Q"),
      new KeyboardKey(this, "W", "W"),
      new KeyboardKey(this, "E", "E"),
      new KeyboardKey(this, "R", "R"),
      new KeyboardKey(this, "T", "T"),
      new KeyboardKey(this, "Y", "Y"),
      new KeyboardKey(this, "U", "U"),
      new KeyboardKey(this, "I", "I"),
      new KeyboardKey(this, "O", "O"),
      new KeyboardKey(this, "P", "P"),
      new KeyboardKey(this, "LeftBrace", "Å"),
      new KeyboardKey(this, "Rightbrace", "^"),
      new KeyboardKey(this, "Enter", "Enter"),
      new KeyboardKey(this, "Delete", "Del"),
      new KeyboardKey(this, "End", "End"),
      new KeyboardKey(this, "PageDown", "PgDn"),

  };

  row3 = {
      new KeyboardKey(this, "CapsLock", "Caps"),
      new KeyboardKey(this, "A", "A"),
      new KeyboardKey(this, "S", "S"),
      new KeyboardKey(this, "D", "D"),
      new KeyboardKey(this, "F", "F"),
      new KeyboardKey(this, "G", "G"),
      new KeyboardKey(this, "H", "H"),
      new KeyboardKey(this, "J", "J"),
      new KeyboardKey(this, "K", "K"),
      new KeyboardKey(this, "L", "L"),
      new KeyboardKey(this, "Backslash", "Ö"),
      new KeyboardKey(this, "Backslash", "Ä"),
      new KeyboardKey(this, "Backslash", "'"),

  };

  row4 = {
      new KeyboardKey(this, "LeftShift", "Shift"),
      new KeyboardKey(this, "Z", "Z"),
      new KeyboardKey(this, "X", "X"),
      new KeyboardKey(this, "C", "C"),
      new KeyboardKey(this, "V", "V"),
      new KeyboardKey(this, "B", "B"),
      new KeyboardKey(this, "N", "N"),
      new KeyboardKey(this, "M", "M"),
      new KeyboardKey(this, "Comma", ","),
      new KeyboardKey(this, "Dot", "."),
      new KeyboardKey(this, "Slash", "-"),
      new KeyboardKey(this, "RightShift", "Shift"),
      new KeyboardKey(this, "Up", "Up"),
  };

  row5 = {
      new KeyboardKey(this, "LeftCtrl", "Ctrl"),
      new KeyboardKey(this, "LeftMeta", "Meta"),
      new KeyboardKey(this, "LeftAlt", "Alt"),
      new KeyboardKey(this, "Space", "Space"),
      new KeyboardKey(this, "RightAlt", "AltGr"),
      new KeyboardKey(this, "Compose", "Menu"),
      new KeyboardKey(this, "RightCtrl", "Ctrl"),
      new KeyboardKey(this, "Left", "Left"),
      new KeyboardKey(this, "Down", "Down"),
      new KeyboardKey(this, "Right", "Right"),
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
