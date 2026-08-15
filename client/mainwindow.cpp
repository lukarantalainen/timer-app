#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QObject>
#include <QSharedMemory>
#include <QSystemTrayIcon>
#include <QTabWidget>
#include <QTimer>
#include <QWidget>

#include "keyboard.h"
#include "logdisplay.h"
#include "statusbar.h"
#include "timer.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("Activity tracker");
  setMinimumSize(100, 100);
  resize(600, 500);

  m_system_tray_icon = createSystemTrayIcon();

  m_timer = new Timer(this);

  m_status_bar = new StatusBar(this);
  setStatusBar(m_status_bar);

  m_log = new LogDisplay(this);
  m_keyboard = new Keyboard(m_status_bar, m_log, this);

  m_central_widget = createCentralWidget();
  setCentralWidget(m_central_widget);
}

QSystemTrayIcon* MainWindow::createSystemTrayIcon() {
  QSystemTrayIcon* tray_icon = new QSystemTrayIcon(this);
  QIcon tray_icon_img(":/images/trayicon.png");
  tray_icon->setIcon(tray_icon_img);
  tray_icon->show();

  QMenu* context_menu = new QMenu(this);
  QAction* quit = new QAction(this);
  quit->setText("Quit");
  quit->setIcon(QIcon::fromTheme("gtk-quit"));

  QObject::connect(quit, &QAction::triggered, this, &QApplication::exit);

  context_menu->addAction(quit);

  tray_icon->setContextMenu(context_menu);

  QObject::connect(tray_icon, &QSystemTrayIcon::activated, this, [this]() {
    if (isVisible())
      hide();
    else
      show();
  });
  return tray_icon;
}

void MainWindow::print(const QString text) { m_log->print(text); }

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
