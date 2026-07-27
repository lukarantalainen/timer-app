#pragma once 

#include <QKeyEvent>
#include <QLabel>
#include <QWidget>
#include <QString>
#include <QChar>

class KeyboardKey : public QWidget {
 public:
  KeyboardKey(QWidget* parent, Qt::Key key);

  void increment() { ++press_count; }
  QLabel* createKeyLabel(QString key);

 private:
  int press_count;
  Qt::Key key;
};

QString QtKeyToChar(Qt::Key key);
