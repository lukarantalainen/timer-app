#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QTabWidget>
#include <QTimer>
#include <QWidget>
#include <iostream>

#include "client.h"
#include "keyboard_heatmap.h"
#include "logdisplay.h"
#include "statusbar.h"
#include "timer.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("Timer app");

  QIcon window_icon{"image.png"};
  setWindowIcon(window_icon);
  setMinimumSize(100, 100);
  resize(600, 500);

  m_client = new Client();

  m_log = new LogDisplay(this);
  m_log->show();

  m_heatmap = new KeyboardHeatmap(KeyboardLayout::QWERTY,
                                  KeyboardSize::SizeTKL80, this);
  m_heatmap->show();

  auto central_widget{centralWidget(this)};
  setCentralWidget(central_widget);

  StatusBar* status_bar = new StatusBar(this);
  setStatusBar(status_bar);

  QObject::connect(m_client, &Client::connectionChanged, status_bar,
                   &StatusBar::connectionChanged);
  QObject::connect(m_client, &Client::connectionCountdown, status_bar, &StatusBar::connectionCountdown);

  QObject::connect(m_client, &Client::keyDown, m_heatmap,
                   &KeyboardHeatmap::keyDown);
  QObject::connect(m_client, &Client::keyUp, m_heatmap,
                   &KeyboardHeatmap::keyUp);

  QObject::connect(m_client, &Client::keyDown, m_log, &LogDisplay::append);
  QObject::connect(m_client, &Client::keyUp, m_log, &LogDisplay::append);
}

QWidget* MainWindow::centralWidget(MainWindow* parent) {
  auto central_widget = new QWidget(this);

  auto tabwidget = new QTabWidget(central_widget);

  auto timer = new Timer(this);

  tabwidget->addTab(m_heatmap, "Keyboard");
  tabwidget->addTab(timer, "Timer");
  tabwidget->addTab(m_log, "Log");
  tabwidget->show();

  central_widget->show();
  return central_widget;
}
