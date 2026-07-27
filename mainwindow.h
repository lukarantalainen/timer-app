#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QWidget>
#include <QLabel>

#include "input.h"

class MainWindow : public QMainWindow {
    public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() = default;

    private:
    QMenuBar* menuBar(MainWindow* parent);
    QWidget* centralWidget(MainWindow* parent);
    
    QLabel* keyboard_label = nullptr;
    Input* input = nullptr;
};
