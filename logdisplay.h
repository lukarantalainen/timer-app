#ifndef LOGDISPLAY_H
#define LOGDISPLAY_H

#include <QPlainTextEdit>
#include <QObject>

class LogDisplay : public QPlainTextEdit {
  Q_OBJECT

  public:

  LogDisplay(QWidget* parent) : QPlainTextEdit(parent) {

  }

  public slots:
    void append(const QString& text);

};

#endif
