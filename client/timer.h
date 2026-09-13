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
class Timer : public QWidget {
  public:
  Timer(QWidget* parent);
  
  void startSession();
  void createSession(QString text);
  void finishSession(Session* session);

  private:
  int elapsed{};
  bool creating = false;

  QLineEdit* line_edit = nullptr;
  QPushButton* start_button = nullptr;
  QGridLayout* layout = nullptr;
  QVBoxLayout* timer_layout = nullptr;

  QWidget* activity_list = nullptr;
  QVBoxLayout* activity_layout = nullptr;
};

class Session : public QWidget {
  public:
  Session(QString text, Timer* parent);

  void playPause();
  QString getName();
  QString getText();
  
  private:
  int session_elapsed{};
  
  bool active = false;
  
  QPushButton* pause_button = nullptr;
  QPushButton* finish_button = nullptr;
  QLabel* label = nullptr;
  QTimer* timer = nullptr;
  QString session_name;
};
