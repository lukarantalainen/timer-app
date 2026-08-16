#include "database.h"
#include <sqlite3.h>
#include <QDebug>

Database::Database() {
  int rc; 
  
  rc = sqlite3_open(":memory", &db);
  if (rc != SQLITE_OK) {
    qDebug() << "ERROR opening SQLite DB in memory: " << sqlite3_errmsg(db);
  }
}

Database::~Database() {
  sqlite3_close(db);
  qDebug() << "Database closed";
}
