#include "timer.h"

#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QGridLayout>

Timer::Timer(QWidget* parent) : QWidget(parent) {
  QGridLayout* layout = new QGridLayout(this);

  QLabel* label = new QLabel(this);
  label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  label->show();

  QTimer* timer = new QTimer();
  timer->setInterval(1000);
  timer->start();

  layout->addWidget(label);

  QPushButton* start_button = new QPushButton("Start session", this);

  layout->addWidget(start_button);

  connect(timer, &QTimer::timeout, this, [this, label]() {
    ++elapsed;
    label->setText(QTime(0,0,0,0).addSecs(elapsed).toString("hh:mm:ss"));
    label->adjustSize();
  });
}
