#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QWidget>
#include <QLabel>

class Input;
class Client;
class KeyboardHeatmap;
class LogDisplay;

class MainWindow : public QMainWindow {
    public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() = default;

    private:
    QWidget* centralWidget(MainWindow* parent);
    
    Client* m_client = nullptr;
    KeyboardHeatmap* m_heatmap = nullptr;
    LogDisplay* m_log = nullptr;
    Input* input = nullptr;
    QStatusBar* status_bar = nullptr;
};
