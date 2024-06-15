# SQLite Database Interaction with C++

## Requirements

- SQLite library (`libsqlite3-dev` on Debian-based systems)
- SQLite C/C++ headers (`sqlite3.h`)


### Functions

- **openSqlite**: Opens an SQLite database.
- **createSqliteTable**: Creates an SQLite table if it does not exist.
- **writeSqlite**: Writes data into an SQLite table.
- **readSqlite**: Reads data from an SQLite table.
- **deleteFromTable**: Deletes data from an SQLite table based on a condition.