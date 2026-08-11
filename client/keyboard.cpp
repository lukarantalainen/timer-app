#include "keyboard.h"

#include <QWidget>

#include "client.h"
#include "keyboard_heatmap.h"
#include "logdisplay.h"
#include "statusbar.h"

Keyboard::Keyboard(StatusBar* status_bar, LogDisplay* log, QWidget* parent) : m_status_bar{status_bar}, m_log{log}, QWidget(parent) {
  m_client = new Client();

  m_heatmap = new KeyboardHeatmap(KeyboardLayout::QWERTY,
                                  KeyboardSize::SizeTKL80, this);

  QObject::connect(m_client, &Client::keyDown, m_heatmap,
                   &KeyboardHeatmap::keyDown);
  QObject::connect(m_client, &Client::keyUp, m_heatmap,
                   &KeyboardHeatmap::keyUp);

  if (m_log) {
    QObject::connect(m_client, &Client::keyDown, m_log, &LogDisplay::append);
    QObject::connect(m_client, &Client::keyUp, m_log, &LogDisplay::append);
  }
  
  if (m_status_bar) {
    QObject::connect(m_client, &Client::connectionChanged, m_status_bar,
                   &StatusBar::connectionChanged);
    QObject::connect(m_client, &Client::connectionCountdown, m_status_bar, &StatusBar::connectionCountdown);
  }
  
  m_client->start();
}

Keyboard::~Keyboard() {
  m_client->stop();
  delete m_client;
  delete m_heatmap;
}
