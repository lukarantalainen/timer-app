#include "menubar.h"

#include <QMenu>
#include <QMenuBar>

#include "keyboard.h"

MenuBar::MenuBar(QWidget* parent) : MenuBar(nullptr, parent) {}

MenuBar::MenuBar(Keyboard* keyboard, QWidget* parent) : keyboard{keyboard}, QMenuBar(parent) {
  QMenu* menu = new QMenu(this);
  menu->setTitle("File");

  QAction* save_action = new QAction("Save", menu);

  QAction* load_action = new QAction("Load", menu);
  
  if (keyboard) {
    QObject::connect(save_action, &QAction::triggered, keyboard, &Keyboard::save);
    QObject::connect(load_action, &QAction::triggered, keyboard, &Keyboard::load);
  } else {
    qDebug() << "nullptr";
  }

  menu->addAction(save_action);
  menu->addAction(load_action);
  
  addMenu(menu);
}
