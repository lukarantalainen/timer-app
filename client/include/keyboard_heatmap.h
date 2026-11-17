#pragma once

#include <QGridLayout>
#include <QObject>
#include <QWidget>
#include <QKeyEvent>

#include <list>
#include <map>
#include <vector>

#include "keyboard_key.h"
#include "keyevent.h"

class Database;

enum class KeyboardLayout {
  QWERTY,
  AZERTY,
  DVORAK,
  COLEMAK,

};

enum class KeyboardSize {
  Size100,
  Size1800,
  SizeCompact96,
  SizeTKL80,
  SizeExploded75,
  SizeCompact75,
  Size65,
  Size60,
  Size50,
  Size40,
};

class KeyboardHeatmap : public QWidget {
  Q_OBJECT
 public:
  KeyboardHeatmap(KeyboardLayout layout, KeyboardSize size, QWidget* parent = nullptr);
  std::vector<std::pair<QString, int>> getKeyData();
  void setValue(QString key, int value);

 private:
  QBoxLayout* m_layout;
  KeyboardLayout m_key_layout;
  KeyboardSize m_size;

  std::list<KeyboardKey*> row0;
  std::list<KeyboardKey*> row1;
  std::list<KeyboardKey*> row2;
  std::list<KeyboardKey*> row3;
  std::list<KeyboardKey*> row4;
  std::list<KeyboardKey*> row5;
  

  std::map<QString, KeyboardKey*> m_keys;

  int min_value{};
  int max_value{};

  void createKeys();
  QBoxLayout* createLayout(std::list<KeyboardKey*> row);

 public slots:
  void keyDown(const KeyEvent& event);
  void keyUp(const KeyEvent& event);
};
