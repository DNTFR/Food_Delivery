#include "database.h"
#include "sqlite3.h"
#include <iostream>
using namespace std;

DatabaseManager::DatabaseManager() : db(nullptr) {}

DatabaseManager::~DatabaseManager() { close(); }

bool DatabaseManager::open(const string& path) {
    int exit = sqlite3_open(path.c_str(), &db);
    if (exit) { 
        cerr << "Error open DB: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    cout << "Opened Database Successfully!" << endl;
    return true;
}

void DatabaseManager::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        cout << "Database closed!" << endl;
    }
}

bool DatabaseManager::execute(const string& sql) {
    char* messageError = nullptr;
    int exit = sqlite3_exec(db, sql.c_str(), nullptr, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert: " << messageError << endl;
        sqlite3_free(messageError);
        return false;
    }
    cout << "Record Creater Successfully!\n";
    return true;
}

static int callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << "--------------------------\n";
    return 0;
}

bool DatabaseManager::selectWithCallback(const string& sql) {
    char* messageError = nullptr;
    int exit = sqlite3_exec(db, sql.c_str(), callback, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Select: " << messageError << endl;
        sqlite3_free(messageError);
        return false;
    }
    return true;
}
