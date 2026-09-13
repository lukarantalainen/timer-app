#include "timer.h"

#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

Timer::Timer(QWidget* parent) : QWidget(parent), layout(new QGridLayout(this)), timer_layout(new QVBoxLayout()) {
  line_edit = new QLineEdit(this);
  line_edit->setPlaceholderText("Session name");
  line_edit->hide();

  start_button = new QPushButton("New session", this);
  connect(start_button, &QPushButton::clicked, this, [this]() {
    if (!creating) {
      creating = true;
      startSession();
    } else {
      createSession(line_edit->text());
      creating = false;
    } 
  });

  layout->addLayout(timer_layout, 0, 0, Qt::AlignTop);
  layout->addWidget(line_edit, 2, 1, Qt::AlignLeft);
  layout->addWidget(start_button, 2, 0, Qt::AlignBottom);

  auto scroll = new QScrollArea(this);
  scroll->setMinimumHeight(300);
  scroll->setMaximumWidth(300);
  scroll->setWidgetResizable(true);
  
  activity_list = new QWidget(this);
  activity_list->setWindowTitle("Completed activities");
  activity_layout = new QVBoxLayout(activity_list);
  activity_list->setLayout(activity_layout);

  scroll->setWidget(activity_list);

  layout->addWidget(scroll, 0, 1, Qt::AlignTop);

  createSession("test");
}

void Timer::startSession() {
  line_edit->show();
  start_button->setText("Start session");
}

void Timer::createSession(QString text) {
  start_button->setText("New session");

  line_edit->clear();
  line_edit->hide();

  Session* session = new Session(text, this);

  timer_layout->addWidget(session);
}

void Timer::finishSession(Session* session) {
  QLabel* label = new QLabel(session->getText());
  activity_layout->addWidget(label);
  timer_layout->removeWidget(session);
  delete session;
}

Session::Session(QString text, Timer* parent) : session_name{text}, timer(new QTimer(this)), label(new QLabel(this)) {

  QHBoxLayout* layout = new QHBoxLayout(this);

  label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  label->setText(session_name + " " + QTime(0,0,0,0).addSecs(session_elapsed).toString("hh:mm:ss"));
  label->adjustSize();

  timer->setInterval(1000);
  
  
  finish_button = new QPushButton("Finish", this);

  pause_button = new QPushButton("Stop", this);

  connect(finish_button, &QPushButton::clicked, parent, [this, parent](){
    parent->finishSession(this);
  });
  connect(pause_button, &QPushButton::clicked, this, &Session::playPause);

  layout->addWidget(label);
  layout->addWidget(pause_button);
  layout->addWidget(finish_button);

  connect(timer, &QTimer::timeout, this, [this]() {
    ++session_elapsed;
    label->setText(session_name + " " + QTime(0,0,0,0).addSecs(session_elapsed).toString("hh:mm:ss"));
    label->adjustSize();
  });

  playPause();
}

QString Session::getName() {
  return session_name;
}

QString Session::getText() {
  return session_name + " " + QTime(0,0,0,0).addSecs(session_elapsed).toString("hh:mm:ss");
}

void Session::playPause() {
  if (timer->isActive()) {
    pause_button->setText("Start");
    label->setStyleSheet("color: white;");
    timer->stop();
  } else {
    pause_button->setText("Stop");
    label->setStyleSheet("color: lightskyblue;");
    timer->start();
  }
}
