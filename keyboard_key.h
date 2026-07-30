#pragma once

#include <QChar>
#include <QColor>
#include <QKeyEvent>
#include <QLabel>
#include <QString>
#include <QWidget>

class KeyboardKey : public QWidget {
 public:
  KeyboardKey(QWidget* parent, Qt::Key key);

  void keyDown();
  void keyUp();
  void setStyle(const QString& style);
  void updateColor(double p);
  int getCount() { return m_count; };

  Qt::Key m_key;

 private:
  int m_count{};
  QLabel* m_label;
  QPalette m_label_palette;

  QLabel* m_count_label;

  const QString m_default_style{
      "border: 1px solid #444; border-radius: 4px;  "
      "color: white; font-weight: bold;"};
  QString m_background_color{"background-color: #333;"};

  QColor color_a = QColor::fromString("green");
  QColor color_b = QColor::fromString("yellow");
  QColor color_c = QColor::fromString("red");

  void increment();
};

QString QtKeyToChar(Qt::Key key);
