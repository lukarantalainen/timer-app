#pragma once

#include <QWidget>
#include <QGridLayout>

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
  

  KeyboardHeatmap(KeyboardLayout layout, KeyboardSize size);
  

  
  private:
  KeyboardLayout layout;
  KeyboardSize size;
};

