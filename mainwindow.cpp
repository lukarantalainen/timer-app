
#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QPalette>
#include <QTimer>
#include <QWidget>
#include <QTabWidget>
#include <iostream>

#include "input.h"
#include "keyboard_heatmap.h"
#include "keyboard_key.h"
#include "logdisplay.h"
#include "timer.h"

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

  QTabWidget* tabwidget = new QTabWidget(central_widget);

  QWidget* total = new QWidget();
  Timer* timer = new Timer(total);

  QWidget* log = new QWidget();

  LogDisplay* logdisplay = new LogDisplay(log);
  logdisplay->show();

  input = new Input(6);
  input->start();

  KeyboardHeatmap* heatmap =
      new KeyboardHeatmap(KeyboardLayout::QWERTY, KeyboardSize::SizeTKL80);
  heatmap->show();

  tabwidget->addTab(heatmap, "Keyboard");
  tabwidget->addTab(timer, "Timer");
  tabwidget->addTab(log, "Log");
  tabwidget->show();




  QObject::connect(input, &Input::keyDown, heatmap, &KeyboardHeatmap::keyDown);
  QObject::connect(input, &Input::keyDown, logdisplay, &LogDisplay::append);

  QObject::connect(input, &Input::keyUp, heatmap, &KeyboardHeatmap::keyUp);

  central_widget->show();
  return central_widget;
}
