
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

#include "keyboard_heatmap.h"
#include "keyboard_key.h"
#include "logdisplay.h"
#include "timer.h"
#include "client.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("Timer app");

  QIcon window_icon{"image.png"};
  setWindowIcon(window_icon);

  auto central_widget{centralWidget(this)};
  central_widget->setFixedSize(600, 500);
  setCentralWidget(central_widget);
  adjustSize();
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

  Client* client = new Client();

  KeyboardHeatmap* heatmap =
      new KeyboardHeatmap(KeyboardLayout::QWERTY, KeyboardSize::SizeTKL80);
  heatmap->show();

  tabwidget->addTab(heatmap, "Keyboard");
  tabwidget->addTab(timer, "Timer");
  tabwidget->addTab(log, "Log");
  tabwidget->show();


  QObject::connect(client, &Client::keyDown, heatmap, &KeyboardHeatmap::keyDown);
  QObject::connect(client, &Client::keyDown, logdisplay, &LogDisplay::append);

  QObject::connect(client, &Client::keyUp, heatmap, &KeyboardHeatmap::keyUp);

  central_widget->show();
  return central_widget;
}
