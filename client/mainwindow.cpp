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

#include "keyboard.h"
#include "logdisplay.h"
#include "statusbar.h"
#include "timer.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("Timer app");

  QIcon window_icon{"image.png"};
  setWindowIcon(window_icon);
  setMinimumSize(100, 100);
  resize(600, 500);

  m_timer = new Timer(this);

  m_status_bar = new StatusBar(this);
  setStatusBar(m_status_bar);

  m_log = new LogDisplay(this);
  m_keyboard = new Keyboard(m_status_bar, m_log, this);

  m_central_widget = createCentralWidget();
  setCentralWidget(m_central_widget);
}

QWidget* MainWindow::createCentralWidget() {
  auto central_widget = new QWidget(this);

  auto tabwidget = new QTabWidget(central_widget);

  auto layout = new QGridLayout(central_widget);
  layout->addWidget(tabwidget);

  tabwidget->addTab(m_keyboard, "Keyboard");
  tabwidget->addTab(m_timer, "Timer");
  tabwidget->addTab(m_log, "Log");

  return central_widget;
}
