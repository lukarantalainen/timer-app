#pragma once

#include <QWidget>

class QWidget;
class Client;
class KeyboardHeatmap;
class LogDisplay;
class StatusBar;

class Keyboard : public QWidget {
  public:
  Keyboard(StatusBar* status_bar, LogDisplay* = nullptr, QWidget* parent = nullptr);
  ~Keyboard();

  private:
  KeyboardHeatmap* m_heatmap = nullptr;
  LogDisplay* m_log = nullptr;
  StatusBar* m_status_bar = nullptr;
  Client* m_client = nullptr;
};
