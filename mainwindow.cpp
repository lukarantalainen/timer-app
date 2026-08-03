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
#include <QTabWidget>
#include <QTimer>
#include <QWidget>
#include <iostream>

#include "client.h"
#include "keyboard_heatmap.h"
#include "logdisplay.h"
#include "timer.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("Timer app");

  QIcon window_icon{"image.png"};
  setWindowIcon(window_icon);
  setMinimumSize(100, 100);
  resize(600, 500);

  auto central_widget{centralWidget(this)};
  setCentralWidget(central_widget);
}

QWidget* MainWindow::centralWidget(MainWindow* parent) {
  auto central_widget = new QWidget(this);

  auto tabwidget = new QTabWidget(central_widget);

  auto timer = new Timer(this);

  auto log = new LogDisplay(this);
  log->show();

  auto client = new Client();

  auto heatmap =
      new KeyboardHeatmap(KeyboardLayout::QWERTY, KeyboardSize::SizeTKL80);
  heatmap->show();

  tabwidget->addTab(heatmap, "Keyboard");
  tabwidget->addTab(timer, "Timer");
  tabwidget->addTab(log, "Log");
  tabwidget->show();

  QObject::connect(client, &Client::keyDown, heatmap,
                   &KeyboardHeatmap::keyDown);
  QObject::connect(client, &Client::keyUp, heatmap, &KeyboardHeatmap::keyUp);

  QObject::connect(client, &Client::keyDown, log, &LogDisplay::append);
  QObject::connect(client, &Client::keyUp, log, &LogDisplay::append);
  
  central_widget->show();
  return central_widget;
}
