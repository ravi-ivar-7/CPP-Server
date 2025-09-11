# Database Module

Provides a simple and reliable way to work with SQLite databases in C++ applications. Handles all common database operations needed.

## What It Does

- Connects to SQLite databases with ease
- Creates tables automatically when they don't exist
- Handles reading, writing, and deleting data safely
- Perfect for storing user data, application settings, and logs

## Available Functions

- **openSqlite()** - Opens a connection to SQLite database
- **createSqliteTable()** - Creates a new table if it doesn't already exist
- **writeSqlite()** - Saves data to database tables
- **readSqlite()** - Retrieves data from database
- **deleteFromTable()** - Removes specific records based on conditions

## Dependencies

- SQLite library (`libsqlite3-dev` on Ubuntu/Debian systems)
- SQLite C/C++ headers (`sqlite3.h`)

Makes database operations straightforward for building great features!