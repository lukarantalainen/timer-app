#include <QApplication>
#include <QSharedMemory>
#include <QString>
#include <QLocalSocket>
#include <QLocalServer>

#include "database.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  // const QString sock = "activitytracker-instance";

  // QSharedMemory shared_memory(sock);

  // if (!shared_memory.create(1)) {
  //   QLocalSocket socket;
  //   socket.connectToServer(sock);

  //   if (!socket.waitForConnected(1000)) return 0;

  //   socket.write("ACTIVATE");
  //   socket.waitForBytesWritten(1000);
  //   return 0;
    
  // }

  MainWindow mainwindow;
  mainwindow.show();

  // QLocalServer server;
  // if (server.listen(sock)) {
  //   QObject::connect(&server, &QLocalServer::newConnection, [&]() {
  //     QLocalSocket* client_socket = server.nextPendingConnection();
  //     QObject::connect(client_socket, &QLocalSocket::readyRead, [&, client_socket]() {
  //       QByteArray data = client_socket->readAll();
  //       if (data == "ACTIVATE") {
  //         mainwindow.print("Another instance launched");
  //         if (mainwindow.isVisible()) {
  //           mainwindow.hide();
  //           mainwindow.show();
  //         }
  //         else mainwindow.show();
  //       }
  //       client_socket->deleteLater();
  //     });
  //   });
  // }

  return app.exec();
}
