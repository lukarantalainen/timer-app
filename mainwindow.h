#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QWidget>

class MainWindow : public QMainWindow {
    public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() = default;

    private:
    QMenuBar* menuBar(MainWindow* parent);
    QWidget* centralWidget(MainWindow* parent);
};

#endif
