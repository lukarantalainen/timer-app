#pragma once 

#include <QKeyEvent>
#include <QLabel>
#include <QWidget>
#include <QString>
#include <QChar>
#include <QKeyEvent>

class KeyboardKey : public QWidget {
 public:
  KeyboardKey(QWidget* parent, Qt::Key key);

  void increment() { ++m_count; }
  void setStyle(const QString& style);

  Qt::Key m_key;
 private:
  int m_count;
  QLabel* m_label;
};

QString QtKeyToChar(Qt::Key key);
