#pragma once 

#include <QPlainTextEdit>
#include <QWidget>
#include <QObject>

class KeyEvent;

enum class Flags {
  DEFAULT = 0x00,
  TIMESTAMP = 0x01,
};


class LogDisplay : public QWidget {
  Q_OBJECT

  public:

  LogDisplay(QWidget* parent);
  void print(const QString text, Flags flags = Flags::DEFAULT);

  public slots:
    void append(const KeyEvent event);

  private:
  QPlainTextEdit* m_textedit;
};

