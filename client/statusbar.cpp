#include "statusbar.h"
#include <QStatusBar>

StatusBar::StatusBar(QWidget* parent) : QStatusBar(parent) {

  //showMessage("hi");

}

void StatusBar::connectionChanged(bool connected) {
  qDebug() << "hello" << "\n";
  if (connected) {
    m_text = "Connected";
  } else {
    m_text = "Disconnected";
  }
  if (this) {
    showMessage("hello");
  }
  
}

