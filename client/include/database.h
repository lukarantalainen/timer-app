#pragma once

#include <string>
#include <vector>
#include <utility>

#include <QKeyEvent>

class sqlite3;

class Database {

  public:
  Database();
  ~Database();
  void update(QString key_name, std::string date, int count);
  void read();
  void saveKeyboard(std::vector<std::pair<QString, int>> keys, std::string date);
  void resetKeyboard(std::string date);
  std::vector<std::pair<QString, int>> loadKeyboard(std::string date);

  private:
  
  sqlite3* db = nullptr;
};
