#include "keyboard_key.h"
#include <QLabel>
#include <QString>
#include <QChar>
#include <QKeyEvent>

KeyboardKey::KeyboardKey(QWidget* parent, Qt::Key key) : QWidget(parent), key{key} {
    QString key_string = QtKeyToChar(key);
    
    QLabel* label = createKeyLabel(key_string);
    label->show();
  }

QLabel* KeyboardKey::createKeyLabel(QString key_name) {
  QLabel* key_label = new QLabel(key_name, this);

  return key_label;

}

QString QtKeyToChar(Qt::Key key) {
  if (key >= Qt::Key_Space && key <= Qt::Key_AsciiTilde) {
    char key_char = static_cast<char>(key);
    return QString(key_char);
  }
  return "";
}
