#include "database.h"
#include <sqlite3.h>

#include <QDebug>
#include <iostream>

Database::Database() {
  int rc; 
  
  rc = sqlite3_open(":memory", &db);
  if (rc != SQLITE_OK) {
    qDebug() << "ERROR opening SQLite DB in memory: " << sqlite3_errmsg(db);
  }
  update();
}

Database::~Database() {
  sqlite3_close(db);
  qDebug() << "Database closed";
}

struct answer {
    std::string str;
  };

static int callback(void *veryUsed, int argc, char **argv, char **azColName){
  int i;
  answer *xyz = (answer *)veryUsed ;
  for(i=0; i<argc; i++){
    if (strcmp(azColName[i], "a") == 0)
       xyz->str = atoi(argv[1]);
  }
  return 0;
}

void Database::update() {
  const char* stmt = "CREATE TABLE Test ("
  "Key TEXT"
  ");"
  "INSERT INTO Test (Key)"
  "VALUES ('Test')";

  int rc = sqlite3_exec(db, stmt, nullptr, nullptr, nullptr);


  stmt = "SELECT * FROM Test";
  
  answer ans;

  rc = sqlite3_exec(db, stmt, nullptr, &ans, nullptr);

  qDebug() << ans.str;
}
