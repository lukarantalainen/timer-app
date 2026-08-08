#pragma once

#include <QStatusBar>
#include <QObject>


class StatusBar : public QStatusBar {
  Q_OBJECT

  public:
  StatusBar(QWidget* parent);


  public slots:
  void connectionChanged(bool connected);

  private:
  QString m_text = "";
};
