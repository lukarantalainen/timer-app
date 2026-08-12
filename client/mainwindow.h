#pragma once

#include <QMainWindow>

class QSystemTrayIcon;
class QWidget;
class StatusBar;
class Keyboard;
class LogDisplay;
class Timer;

class MainWindow : public QMainWindow {
 public:
  MainWindow(QWidget* parent = nullptr);
  virtual ~MainWindow() = default;

 private:
  QWidget* createCentralWidget();
  QSystemTrayIcon* createSystemTrayIcon();

  QSystemTrayIcon* m_system_tray_icon = nullptr;
  QWidget* m_central_widget = nullptr;
  StatusBar* m_status_bar = nullptr;
  LogDisplay* m_log = nullptr;
  Keyboard* m_keyboard = nullptr;
  Timer* m_timer = nullptr;
};
