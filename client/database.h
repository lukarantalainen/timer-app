#pragma once

class sqlite3;

class Database {

  public:
  Database();
  ~Database();

  private:

  sqlite3* db = nullptr;


};
