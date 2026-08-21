#pragma once

class sqlite3;

class Database {

  public:
  Database();
  ~Database();
  void update();

  private:

  sqlite3* db = nullptr;


};
