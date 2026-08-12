#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QLocalServer>
#include <QLocalSocket>

#include "mainwindow.h"

void handleConnection() {

}

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QLocalServer server;

  if (!server.listen("activitytracker")) {
    QLocalSocket* socket = new QLocalSocket();
    socket->connectToServer("activitytracker");
  }

  MainWindow mainwindow;
  mainwindow.show();

  return app.exec();
}
