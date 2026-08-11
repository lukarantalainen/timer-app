#include "statusbar.h"
#include <QStatusBar>
#include <QLabel>

StatusBar::StatusBar(QWidget* parent) : QStatusBar(parent) {

  status_label = new QLabel();
  status_label->setText("test");
  status_label->show();
  addPermanentWidget(status_label, 1);

  count_label = new QLabel();
  count_label->show();
  addPermanentWidget(count_label, 1);
}

StatusBar::~StatusBar() {
  delete status_label;
  delete count_label;
}

void StatusBar::connectionCountdown(int value) {
  count_label->setText("Trying to connect: " + QString::number(value));
  count_label->adjustSize();
}

void StatusBar::connectionChanged(bool connected) {
  if (!status_label) return;
  if (connected) {
    status_label->setText("Connected");
    count_label->hide();
  } else {
    status_label->setText("Disconnected");
    count_label->show();
  }
}

