#pragma once

#include <QWidget>
#include <QTimer>
#include <QLabel>

class QGridLayout;
class QPushButton;
class QLineEdit;
class QString;
class QVBoxLayout;

class Session;

struct Activity {
  QString name;
  int duration;
};

class Timer : public QWidget {
  public:
  Timer(QWidget* parent);
  
  void startSession();
  void createSession(QString text);
  void finishSession(Session* session);

  private:
  int elapsed{};
  bool creating = false;

  std::vector<Activity> activities;

  QLineEdit* line_edit = nullptr;
  QPushButton* start_button = nullptr;
  QVBoxLayout* timer_layout = nullptr;

  QVBoxLayout* activity_layout = nullptr;
};

class Session : public QWidget {
  public:
  Session(QString text, Timer* parent);

  void playPause();
  int getElapsed();
  QString getName();
  QString getText();
  void update();
  
  private:
  int session_elapsed{};
  
  bool active = false;
  
  QPushButton* pause_button = nullptr;
  QPushButton* finish_button = nullptr;
  QLabel* label = nullptr;
  QTimer* timer = nullptr;
  QString session_name;
};
