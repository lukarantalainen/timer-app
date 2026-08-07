#include "timer.h"

#include <QLabel>
#include <QTimer>
#include <QTime>

Timer::Timer(QWidget* parent) : QWidget(parent) {

  QLabel* label = new QLabel(this);
  label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  label->show();

  QTimer* timer = new QTimer();
  timer->setInterval(1000);
  timer->start();

  connect(timer, &QTimer::timeout, this, [this, label]() {
    ++elapsed;
    label->setText(QTime(0,0,0,0).addSecs(elapsed).toString("hh:mm:ss"));
    label->adjustSize();
  });
}
