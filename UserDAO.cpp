#include "DAO.h"
#include <iostream>
#include <sstream>
using namespace std;

UserDAO::UserDAO(DatabaseManager& database) : db(database) {}

bool UserDAO::Insert(const User& user) {
    stringstream sql;
    string roleStr;
    switch(user.Getrole()) {
        case Admin: roleStr = "Admin"; break;
        case Manager: roleStr = "Manager"; break;
        default: roleStr = "Customer";
    }
    
    sql << "INSERT INTO users (id, username, password, role, phone) VALUES ("
        << user.GetID() << ", '"
        << user.Getname() << "', '"
        << "1234', '" 
        << roleStr << "', '"
        << user.GetPhone() << "');";
    return db.execute(sql.str());
}

bool UserDAO::Update(const User& user) {
    stringstream sql;
    string roleStr;
    switch(user.Getrole()) {
        case Admin: roleStr = "Admin"; break;
        case Manager: roleStr = "Manager"; break;
        default: roleStr = "Customer";
    }
    
    sql << "UPDATE users SET "
        << "username = '" << user.Getname() << "', "
        << "role = '" << roleStr << "', "
        << "phone = '" << user.GetPhone() << "' "
        << "WHERE id = " << user.GetID() << ";";
    return db.execute(sql.str());
}

bool UserDAO::Remove(int id) {
    stringstream sql;
    sql << "DELETE FROM users WHERE id = " << id << ";";
    return db.execute(sql.str());
}

int UserDAO::UserCallback(void* data, int argc, char** argv, char** azColName) {
    vector<User>* users = static_cast<vector<User>*>(data);
    
    int id = argv[0] ? atoi(argv[0]) : 0;
    string username = argv[1] ? argv[1] : "";
    string password = argv[2] ? argv[2] : "";
    string roleStr = argv[3] ? argv[3] : "Customer";
    string phone = argv[4] ? argv[4] : "";
    
    Role role;
    if (roleStr == "Admin") role = Admin;
    else if (roleStr == "Manager") role = Manager;
    else role = Customer;
    
    User user(id, username, role, phone);
    users->push_back(user);
    
    return 0;
}

User* UserDAO::FindById(int id) {
    stringstream sql;
    sql << "SELECT * FROM users WHERE id = " << id << ";";
    vector<User> results;
    if (db.query(sql.str(), UserCallback, &results)) {
        if (!results.empty()) {
            return new User(results[0]);
        }
    }
    return nullptr;
}

User* UserDAO::FindByUsername(const string& username) {
    stringstream sql;
    sql << "SELECT * FROM users WHERE username = '" << username << "';";
    vector<User> results;
    if (db.query(sql.str(), UserCallback, &results)) {
        if (!results.empty()) {
            return new User(results[0]);
        }
    }
    return nullptr;
}

vector<User> UserDAO::FindAll() {
    vector<User> results;
    string sql = "SELECT * FROM users;";
    db.query(sql, UserCallback, &results);
    return results;
}

bool UserDAO::authenticate(const string& username, const string& password) {
    stringstream sql;
    sql << "SELECT * FROM users WHERE username = '" << username 
        << "' AND password = '" << password << "';";
    
    vector<User> results;
    if (db.query(sql.str(), UserCallback, &results)) {
        if (!results.empty()) {
            cout << " Login successful! Welcome " << username << endl;
            return true;
        } else {
            cout << " Invalid username or password!" << endl;
        }
    }
    return false;
}