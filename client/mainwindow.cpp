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
#include "database.h"
#include "menubar.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), database(new Database) {
  setWindowTitle("Activity tracker");
  setMinimumSize(100, 100);
  resize(600, 500);

  system_tray_icon = createSystemTrayIcon();

  timer = new Timer(this);

  status_bar = new StatusBar(this);
  setStatusBar(status_bar);

  log_display = new LogDisplay(this);
  keyboard = new Keyboard(status_bar, log_display, this, database);

  MenuBar* menubar = new MenuBar(keyboard, this);
  setMenuBar(menubar);

  central_widget = createCentralWidget();
  setCentralWidget(central_widget);
}

MainWindow::~MainWindow() {
  delete database;
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

void MainWindow::print(const QString text) { log_display->print(text); }

QWidget* MainWindow::createCentralWidget() {
  auto central_widget = new QWidget(this);

  auto tabwidget = new QTabWidget(central_widget);

  auto layout = new QGridLayout(central_widget);
  layout->addWidget(tabwidget);

  tabwidget->addTab(keyboard, "Keyboard");
  tabwidget->addTab(timer, "Timer");
  tabwidget->addTab(log_display, "Log");

  return central_widget;
}
