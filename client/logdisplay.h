#pragma once 

#include <QPlainTextEdit>
#include <QWidget>
#include <QObject>

class KeyEvent;
class LogDisplay : public QWidget {
  Q_OBJECT

  public:

  LogDisplay(QWidget* parent);

  public slots:
    void append(const KeyEvent event);

  private:
  QPlainTextEdit* m_textedit;
};

