#ifndef DATABASE_H
#define DATABASE_H
#include "sqlite3.h"
#include <iostream>
using namespace std;

class DatabaseManager {
    public:
        DatabaseManager();
        
        bool open(const string& path);
        void close();
        
        bool execute(const string& sql);
        bool selectWithCallback(const string& sql);        

        ~DatabaseManager();
    private:
        sqlite3* db;
};

#endif