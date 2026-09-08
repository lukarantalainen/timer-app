#include "keyboard.h"

#include <QWidget>

#include "client.h"
#include "keyboard_heatmap.h"
#include "logdisplay.h"
#include "statusbar.h"
#include "database.h"

Keyboard::Keyboard(StatusBar* status_bar, LogDisplay* log, QWidget* parent, Database* database) : status_bar{status_bar}, log{log}, QWidget(parent), database(database) {
  client = new Client();

  heatmap = new KeyboardHeatmap(KeyboardLayout::QWERTY,
                                  KeyboardSize::SizeTKL80, this);

  QObject::connect(client, &Client::keyDown, heatmap,
                   &KeyboardHeatmap::keyDown);
  QObject::connect(client, &Client::keyUp, heatmap,
                   &KeyboardHeatmap::keyUp);

  if (log) {
    QObject::connect(client, &Client::keyDown, log, &LogDisplay::append);
    QObject::connect(client, &Client::keyUp, log, &LogDisplay::append);
  }
  
  if (status_bar) {
    QObject::connect(client, &Client::connectionChanged, status_bar,
                   &StatusBar::connectionChanged);
    QObject::connect(client, &Client::connectionCountdown, status_bar, &StatusBar::connectionCountdown);
  }
  
  client->start();
  load();
}

Keyboard::~Keyboard() {
  client->stop();
  delete client;
  delete heatmap;
}

std::string getDate() {
  std::time_t rawtime;
  std::tm* timeinfo;
  char buffer[80];
  
  std::time(&rawtime);
  timeinfo = std::localtime(&rawtime);

  std::strftime(buffer, 80, "%Y-%m-%d", timeinfo);
  std::string date(buffer);

  return date;
}

void Keyboard::save() {
  if (database && heatmap) {
    database->saveKeyboard(heatmap->getKeyData(), getDate());
    log->print("Saved to database");
  }
}

void Keyboard::load() {
  auto data = database->loadKeyboard(getDate());

  for (auto p : data) {
    heatmap->setValue(p.first, p.second);
  }
}

void Keyboard::resetDatabase() {
  database->resetKeyboard(getDate());
  
  load();
}
