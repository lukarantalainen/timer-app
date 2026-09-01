#include "database.h"

#include <sqlite3.h>

#include <QDebug>
#include <QKeyEvent>
#include <QKeySequence>
#include <QString>
#include <string>
#include <vector>
#include <ctime>
#include <cstdio>

Database::Database() {
  int rc = sqlite3_open("database", &db);
  if (rc != SQLITE_OK) {
    qDebug() << "SQL Error open: " << sqlite3_errmsg(db);
    return;
  }

  char* error;
  rc = sqlite3_exec(db,
                    "CREATE TABLE IF NOT EXISTS keyboard ("
                    "key_name TEXT, date TEXT NOT NULL, count INT NOT NULL "
                    "DEFAULT 0, PRIMARY KEY (key_name, date));",
                    nullptr, nullptr, &error);
  if (rc != SQLITE_OK) {
    qDebug() << "SQL Error exec:" << sqlite3_errmsg(db);
    sqlite3_free(error);
  }

  update("Key_A", "2026-08-29", 27);
}

Database::~Database() {
  sqlite3_close(db);
  qDebug() << "Database closed";
}

struct answer {
  std::string str;
};

int callback(void* unused, int count, char** data, char** column) {
  for (int i = 0; i < count; ++i) {
    if (data[i] && column[i]) {
      qDebug() << column[i] << ":" << data[i];
    }
  }
  return 0;
}

void Database::read() {
  answer ans;
  char* error = nullptr;
  int rc = sqlite3_exec(db, "SELECT * FROM keyboard;", callback, &ans, &error);
  if (rc != SQLITE_OK) {
    qDebug() << "read:" << sqlite3_errmsg(db);
    sqlite3_free(error);
  }
}

void Database::update(std::string key_name, std::string date, int count) {
  char* error;

  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db,
                              "INSERT INTO keyboard (key_name, date, count) "
                              "VALUES (?, ?, ?) ON CONFLICT (key_name, date) "
                              "DO UPDATE SET count = count + excluded.count",
                              -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    qDebug() << "SQL prepare:" << sqlite3_errmsg(db);
  }

  rc = sqlite3_bind_text(stmt, 1, key_name.c_str(), key_name.size(), nullptr);
  if (rc != SQLITE_OK) {
    qDebug() << "SQL bind:" << sqlite3_errmsg(db);
  }
  rc = sqlite3_bind_text(stmt, 2, date.c_str(), date.size(), nullptr);
  if (rc != SQLITE_OK) {
    qDebug() << "SQL bind:" << sqlite3_errmsg(db);
  }
  rc = sqlite3_bind_int(stmt, 3, count);
  if (rc != SQLITE_OK) {
    qDebug() << "SQL bind:" << sqlite3_errmsg(db);
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    read();
  }

  rc = sqlite3_finalize(stmt);
  if (rc != SQLITE_OK) {
    qDebug() << "SQL finalize:" << sqlite3_errmsg(db);
  }
}

void Database::saveKeyboard(std::vector<std::pair<Qt::Key, int>> keys, std::string date) {
  for (auto key : keys) {
    std::string key_name = QKeySequence(key.first).toString().toStdString();
    qDebug() << key_name << date << key.second;
    update(key_name, date, key.second);
  }
}

std::vector<std::pair<Qt::Key, int>> Database::loadKeyboard(std::string date) {
  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db, "SELECT * FROM keyboard WHERE date = ?", -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    qDebug() << "SQL prepare:" << sqlite3_errmsg(db);
  }

  rc = sqlite3_bind_text(stmt, 1, date.c_str(), date.size(), nullptr);

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    const unsigned char* key = sqlite3_column_text(stmt, 0);
    int count = sqlite3_column_int(stmt, 2);

    std::string key_name(reinterpret_cast<const char*>(key));

    qDebug() << key_name << count;
  }

  rc = sqlite3_finalize(stmt);

  return {};
}
