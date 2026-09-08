#pragma once

#include <QWidget>

class QWidget;
class Client;
class KeyboardHeatmap;
class LogDisplay;
class StatusBar;
class Database;

class Keyboard : public QWidget {
  public:
  Keyboard(StatusBar* status_bar, LogDisplay* = nullptr, QWidget* parent = nullptr, Database* database = nullptr);
  ~Keyboard();

  void save();
  void load();
  void resetDatabase();

  private:
  Database* database = nullptr;
  KeyboardHeatmap* heatmap = nullptr;
  LogDisplay* log = nullptr;
  StatusBar* status_bar = nullptr;
  Client* client = nullptr;
};
