#include "timer.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

Timer::Timer(QWidget* parent)
    : QWidget(parent),
      timer_layout(new QVBoxLayout()) {
  QLabel* session_label = new QLabel("Sessions");
  QLabel* finished_label = new QLabel("Finished activities");

  

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

  auto scroll = new QScrollArea(this);
  scroll->setWidgetResizable(true);

  QWidget* activity_list = new QWidget(this);
  activity_list->setWindowTitle("Completed activities");
  activity_layout = new QVBoxLayout(activity_list);
  activity_layout->addStretch();
  activity_list->setLayout(activity_layout);
  scroll->setWidget(activity_list);

  QWidget* timer = new QWidget(this);
  timer->setLayout(timer_layout);

  auto* layout = new QGridLayout(this);

  layout->addWidget(session_label, 0, 0, Qt::AlignTop);
  layout->setColumnStretch(0, 1);
  layout->addWidget(finished_label, 0, 1, Qt::AlignTop);
  layout->setColumnStretch(1, 1);

  layout->addWidget(timer, 1, 0, Qt::AlignTop);
  layout->addWidget(scroll, 1, 1);

  layout->addWidget(start_button, 2, 0, Qt::AlignLeft);
  layout->addWidget(line_edit, 2, 0, Qt::AlignRight);
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

  timer_layout->addWidget(session, Qt::AlignTop);
}

void Timer::finishSession(Session* session) {
  QLabel* label = new QLabel(session->getText());
  activity_layout->addWidget(label, Qt::AlignTop);
  timer_layout->removeWidget(session);
  activities.push_back(Activity{session->getName(), session->getElapsed()});
  delete session;
}

void Session::update() {
  label->setText(
      session_name + "\n" +
      QTime(0, 0, 0, 0).addSecs(session_elapsed).toString("hh:mm:ss"));
  label->adjustSize();
}

Session::Session(QString text, Timer* parent)
    : session_name{text}, timer(new QTimer(this)), label(new QLabel(this)) {
  QHBoxLayout* layout = new QHBoxLayout(this);

  label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

  timer->setInterval(1000);

  finish_button = new QPushButton("Finish", this);

  pause_button = new QPushButton("Stop", this);

  connect(finish_button, &QPushButton::clicked, parent,
          [this, parent]() { parent->finishSession(this); });
  connect(pause_button, &QPushButton::clicked, this, &Session::playPause);

  layout->addWidget(label);
  layout->addWidget(pause_button);
  layout->addWidget(finish_button);

  connect(timer, &QTimer::timeout, this, [this]() {
    ++session_elapsed;
    update();
  });

  update();
  playPause();
}

int Session::getElapsed() { return session_elapsed; }

QString Session::getName() { return session_name; }

QString Session::getText() {
  return session_name + " " +
         QTime(0, 0, 0, 0).addSecs(session_elapsed).toString("hh:mm:ss");
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
