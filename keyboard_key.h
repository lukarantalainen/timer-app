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
  void setDefaultStyle();

  Qt::Key m_key;
 private:
  int m_count;
  QLabel* m_label;
  const QString m_default_style{"border: 1px solid #444; border-radius: 4px; background-color: #333; "
      "color: white; font-weight: bold;"};
};

QString QtKeyToChar(Qt::Key key);
