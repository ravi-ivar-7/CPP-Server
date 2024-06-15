#ifndef SRC_DATABASES_SQLITE
#define SRC_DATABASES_SQLITE

#include <sqlite3.h>
#include <iostream>
#include <future>
#include <vector>
#include <string>
#include <unordered_map>

class Sqlite
{
public:
    std::future<int> openSqlite(sqlite3 **db, const std::string &dbName);

    // CREATE TABLE IF NOT EXISTS table_name (
    // 	contact_id INTEGER PRIMARY KEY,
    // 	first_name TEXT NOT NULL,
    // 	last_name TEXT NOT NULL,
    // 	email TEXT NOT NULL UNIQUE,
    // 	phone TEXT NOT NULL UNIQUE
    // );
    std::future<int> createSqliteTable(sqlite3 **db, const std::string &tableQuery);

    // "INSERT INTO table_name (column1,column2 ,..) VALUES( value1,	value2 ,...);"
    std::future<int> writeSqlite(sqlite3 **db, const std::string &writeQuery);

    // "SELECT * FROM table_name;"
    std::future<std::vector<std::unordered_map<std::string, std::string>>> readSqlite(sqlite3 **db, const std::string &readQuery);

    // "DELETE FROM table_name WHERE delete_condition;"
    int deleteFromTable(sqlite3 *db, const std::string& deleteQuery);
};

#endif // SRC_DATABASES_SQLITE