#include <sqlite3.h>
#include <iostream>
#include <future>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

#include "sqlite.hpp"

std::future<int> Sqlite::openSqlite(sqlite3** db, const std::string& dbName) {
    return std::async(std::launch::async, [db, dbName]() {
        int rc = sqlite3_open(dbName.c_str(), db);
        if (rc) {
            std::ostringstream errMsg;
            errMsg << "Cannot open DB: " << sqlite3_errmsg(*db);
            throw std::runtime_error(errMsg.str());
        }
        return rc;
    });
}

std::future<int> Sqlite::createSqliteTable(sqlite3** db, const std::string& tableQuery) {
    return std::async(std::launch::async, [db, tableQuery]() {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(*db, tableQuery.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::string error = "CREATE TABLE ERROR: " + std::string(errMsg);
            sqlite3_free(errMsg);
            throw std::runtime_error(error);
        }
        return rc;
    });
}

std::future<int> Sqlite::writeSqlite(sqlite3** db, const std::string& writeQuery) {
    return std::async(std::launch::async, [db, writeQuery]() {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(*db, writeQuery.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::string error = "WRITE ERROR: " + std::string(errMsg);
            sqlite3_free(errMsg);
            throw std::runtime_error(error);
        }
        return rc;
    });
}

int Sqlite::deleteFromTable(sqlite3 *db, const std::string& deleteQuery) {
    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, deleteQuery.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::string error = "DELETE ERROR: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }

    return rc;
}

std::future<std::vector<std::unordered_map<std::string, std::string>>> Sqlite::readSqlite(sqlite3** db, const std::string& readQuery) {
    return std::async(std::launch::async, [db, readQuery]() {
        sqlite3_stmt* stmt;
        std::vector<std::unordered_map<std::string, std::string>> result;
        int rc = sqlite3_prepare_v2(*db, readQuery.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("FAILED TO EXECUTE QUERY: " + std::string(sqlite3_errmsg(*db)));
        }

        int colCnt = sqlite3_column_count(stmt);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::unordered_map<std::string, std::string> row;
            for (int i = 0; i < colCnt; i++) {
                std::string colName = sqlite3_column_name(stmt, i);
                const unsigned char* colValue = sqlite3_column_text(stmt, i);
                row[colName] = colValue ? reinterpret_cast<const char*>(colValue) : "NULL";
            }
            result.push_back(row);
        }

        sqlite3_finalize(stmt);
        return result;
    });
}



// g++ -o sqlite sqlite.cpp -lsqlite3

int useSqlite() {
    Sqlite sqlite;
    sqlite3* db;
    std::string dbName = "../../sqliteDB.db";

    try {
        auto openFuture = sqlite.openSqlite(&db, dbName);
        openFuture.get(); // Ensure the database is opened successfully

        std::string tableQuery = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT, phone TEXT);";
        auto createTableFuture = sqlite.createSqliteTable(&db, tableQuery);
        createTableFuture.get(); // Ensure the table is created successfully

        std::string writeQuery = "INSERT INTO test (name, phone) VALUES ('LOL', '45678765456');";
        auto writeFuture = sqlite.writeSqlite(&db, writeQuery);
        writeFuture.get(); // Ensure the data is written successfully

        std::string readQuery = "SELECT * FROM test;";
        auto readFuture = sqlite.readSqlite(&db, readQuery);
        auto data = readFuture.get(); // Retrieve the data

        for (const auto& row : data) {
            for (const auto& [column, value] : row) {
                std::cout << column << ": " << value << std::endl;
            }
            std::cout << std::endl;
        }

    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    sqlite3_close(db);
    return 0;
}
