#pragma once

#include <QWidget>
class Timer : public QWidget {
  public:
  Timer(QWidget* parent);


  private:
  int elapsed{};
};


