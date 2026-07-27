#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPalette>
#include <QWidget>
#include <QLabel>
#include <QElapsedTimer>
#include <QTimer>
#include <QObject>

#include "timer.h"
#include "input.h"
#include "logdisplay.h"
#include "keyboard_key.h"

#include <iostream>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  resize(500, 500);
  setWindowTitle("Timer app");
  
  QIcon window_icon{"image.png"};
  setWindowIcon(window_icon);

  auto central_widget{centralWidget(this)};
  setCentralWidget(central_widget);
}

QMenuBar* MainWindow::menuBar(MainWindow* parent) {
  QMenuBar* menubar = new QMenuBar(parent);
  QMenu* filemenu = new QMenu(menubar);
  filemenu->setTitle("File");

  menubar->addMenu(filemenu);
  menubar->show();
  return menubar;
}

void updateLabel(QLabel* label, QElapsedTimer::Duration num) {
  double duration = static_cast<double>(num.count());
  label->setNum(duration);
}

QWidget* MainWindow::centralWidget(MainWindow* parent) {
  QWidget* central_widget = new QWidget(this);
  QGridLayout* layout = new QGridLayout();

  QWidget* total = new QWidget();

  Timer* timer = new Timer(total);
  layout->addWidget(timer);
  
  QWidget* keyboard = new QWidget();
  layout->addWidget(keyboard);

  keyboard_label = new QLabel(keyboard);
  keyboard_label->setText("keys pressed:");
  keyboard_label->show();

  LogDisplay* logdisplay = new LogDisplay(keyboard);
  logdisplay->show();

  input = new Input(6);
  input->start();

  KeyboardKey* key = new KeyboardKey(central_widget, Qt::Key_A);
  key->show();

  QObject::connect(input, &Input::keyPressed,
                   logdisplay, &LogDisplay::append);
  
  central_widget->setLayout(layout);
  central_widget->show();
  return central_widget;
}
