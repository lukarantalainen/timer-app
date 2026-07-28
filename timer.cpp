#include "timer.h"

#include <QLabel>
#include <QElapsedTimer>
#include <QTimer>

Timer::Timer(QWidget* parent) : QWidget(parent) {

  QLabel* label = new QLabel(this);
  label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  label->show();

  QElapsedTimer* elapsed_timer = new QElapsedTimer();
  elapsed_timer->start();
  QTimer* timer = new QTimer();
  timer->setInterval(1000);
  timer->start();

  connect(timer, &QTimer::timeout, this, [elapsed_timer, label]() {
    auto elapsed = std::chrono::duration<double>(elapsed_timer->durationElapsed()).count();
    label->setNum(static_cast<int>(elapsed));
    label->adjustSize();
  });
}
