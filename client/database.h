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
  void update(std::string key, std::string date, int count);
  void read();
  void saveKeyboard(std::vector<std::pair<Qt::Key, int>> keys, std::string date);
  std::vector<std::pair<Qt::Key, int>> loadKeyboard(std::string date);

  private:
  
  sqlite3* db = nullptr;
};
