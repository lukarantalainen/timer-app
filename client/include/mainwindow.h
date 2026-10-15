#pragma once

#include <QMainWindow>

class QSystemTrayIcon;
class QWidget;
class StatusBar;
class Keyboard;
class LogDisplay;
class Timer;
class Database;

class MainWindow : public QMainWindow {
 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  void print(const QString text);

  

 private:
  QWidget* createCentralWidget();
  QSystemTrayIcon* createSystemTrayIcon();
  
  Database* database;

  QSystemTrayIcon* system_tray_icon = nullptr;
  QWidget* central_widget = nullptr;
  StatusBar* status_bar = nullptr;
  LogDisplay* log_display = nullptr;
  Keyboard* keyboard = nullptr;
  Timer* timer = nullptr;
};
