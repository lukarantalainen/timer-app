#pragma once

#include <QStatusBar>
#include <QObject>

class QLabel;
class StatusBar : public QStatusBar {
  Q_OBJECT

  public:
  StatusBar(QWidget* parent);


  public slots:
  void connectionChanged(bool connected);

  private:
  QLabel* status_label = nullptr;
};
