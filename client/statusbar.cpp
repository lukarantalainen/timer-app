#include "statusbar.h"
#include <QStatusBar>
#include <QLabel>

StatusBar::StatusBar(QWidget* parent) : QStatusBar(parent) {

  status_label = new QLabel();
  status_label->setText("test");
  status_label->show();
  addPermanentWidget(status_label, 1);

}

void StatusBar::connectionChanged(bool connected) {
  if (!status_label) return;
  if (connected) {
    status_label->setText("Connected");
  } else {
    status_label->setText("Disconnected");
  }
}

