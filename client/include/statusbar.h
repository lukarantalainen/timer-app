#pragma once

#include <QStatusBar>
#include <QObject>

class QLabel;
class StatusBar : public QStatusBar {
  Q_OBJECT

  public:
  StatusBar(QWidget* parent);
  ~StatusBar();

  public slots:
  void connectionChanged(bool connected);
  void connectionCountdown(double value);

  private:
  QLabel* status_label = nullptr;
  QLabel* count_label = nullptr;
};
