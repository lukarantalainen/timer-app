#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPalette>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  resize(500, 500);
  setWindowTitle("Timer app");

  QIcon window_icon{"image.png"};
  setWindowIcon(window_icon);

  auto central_widget{centralWidget(this)};
  setCentralWidget(central_widget);

  auto menubar{menuBar(this)};
  setMenuBar(menubar);
}

QMenuBar* MainWindow::menuBar(MainWindow* parent) {
  QMenuBar* menubar = new QMenuBar(parent);
  QMenu* filemenu = new QMenu(menubar);
  filemenu->setTitle("File");

  QAction* open_file = new QAction("&Open File");
  open_file->setShortcut(QKeySequence::Open);
  open_file->setStatusTip("Open an existing file");
  connect(open_file, &QAction::triggered, parent, &MainWindow::open);

  filemenu->addAction(open_file);

  menubar->addMenu(filemenu);
  menubar->show();
  return menubar;
}

void MainWindow::open() {
  QFileDialog::getOpenFileName(
      this, tr("Open Document"), QDir::currentPath(),
      tr("Document files (*.doc *.rtf);;All files (*.*)"), 0,
      QFileDialog::DontUseNativeDialog);
}

QWidget* MainWindow::centralWidget(MainWindow* parent) {
  QWidget* central_widget = new QWidget(this);
  QGridLayout* layout = new QGridLayout();
  QWidget* total = new QWidget();
  layout->addWidget(total);
  QWidget* keyboard = new QWidget();
  layout->addWidget(keyboard);
  central_widget->setLayout(layout);
  central_widget->show();
  return central_widget;
}
