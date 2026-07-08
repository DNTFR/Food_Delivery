#include "DAO.h"
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

UserDAO::UserDAO(DatabaseManager& database) : db(database) {}

int UserDAO::UserCallback(void* data, int argc, char** argv, char** azColName) {
    auto* usersList = static_cast<vector<User*>*>(data);
    if (argc >= 6 && argv[0] && argv[1] && argv[2] && argv[3]) {
        int id = atoi(argv[0]);
        string username = argv[1];
        string password = argv[2];
        int roleInt = atoi(argv[3]);
        int points = argv[4] ? atoi(argv[4]) : 0;
        int restId = argv[5] ? atoi(argv[5]) : -1;
        Role role = static_cast<Role>(roleInt);
        User* user = nullptr;
        
        if (role == Customer) {
            user = new CustomerUser(id, username, password, points);
        } 
        else if (role == Manager) user = new User(id, username, password, Manager);
        else user = new User(id, username, password, Admin);
        
        if (user) usersList->push_back(user);
    }
    return 0;
}

vector <User*> UserDAO::FindAll() {
    string sql = "SELECT id, username, password, role, points, restaurant_id FROM users;";
    vector <User*> results;
    db.query(sql, UserCallback, &results);
    return results;
}

bool UserDAO::Insert(User* user) {
    if (!user) return false;
    stringstream sql;
    
    int points = 0;
    if (user->GetRole() == Customer) {
        CustomerUser* cust = dynamic_cast<CustomerUser*>(user);
        if (cust) points = cust->GetPoints();
    }
    
    sql << "INSERT INTO users (username, password, role, points, restaurant_id) VALUES ('"
        << user->GetName() << "', '"
        << user->GetPass() << "', "
        << static_cast<int>(user->GetRole()) << ", "
        << points << ", NULL);";
        
    return db.execute(sql.str());
}

bool UserDAO::UpdatePoints(int userId, int points) {
    stringstream sql;
    sql << "UPDATE users SET points = " << points << " WHERE id = " << userId << ";";
    return db.execute(sql.str());
}