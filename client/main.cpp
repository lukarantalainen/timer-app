#include <iostream>
#include "timer.h"

#include <QApplication>
#include <QWidget>
#include "mainwindow.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QRadioButton>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainwindow;
    mainwindow.show();

    return app.exec();
}
    