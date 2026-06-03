#include "DAO.h"
#include <iostream>
#include <sstream>
using namespace std;

RestaurantDAO::RestaurantDAO(DatabaseManager& database) : db(database) {}

bool RestaurantDAO::Insert(const Restaurant& restaurant) {
    stringstream sql;
    sql << "INSERT INTO restaurants (id, name, phone, prep_time, description) VALUES ("
        << restaurant.GetID() << ", '"
        << restaurant.Getname() << "', '"
        << restaurant.GetPhone() << "', "
        << restaurant.GetPrep() << ", '"
        << restaurant.Getdesc() << "');";
    return db.execute(sql.str());
}

bool RestaurantDAO::Update(const Restaurant& restaurant) {
    stringstream sql;
    sql << "UPDATE restaurants SET "
        << "name = '" << restaurant.Getname() << "', "
        << "phone = '" << restaurant.GetPhone() << "', "
        << "prep_time = " << restaurant.GetPrep() << ", "
        << "description = '" << restaurant.Getdesc() << "' "
        << "WHERE id = " << restaurant.GetID() << ";";
    return db.execute(sql.str());
}

bool RestaurantDAO::Remove(int id) {
    stringstream sql;
    sql << "DELETE FROM restaurants WHERE id = " << id << ";";
    return db.execute(sql.str());
}

int RestaurantDAO::RestaurantCallback(void* data, int argc, char** argv, char** azColName) {
    vector<Restaurant>* restaurants = static_cast<vector<Restaurant>*>(data);
    int id = argv[0] ? atoi(argv[0]) : 0;
    string name = argv[1] ? argv[1] : "";
    string address = argv[2] ? argv[2] : "";
    string phone = argv[3] ? argv[3] : "";
    string status = argv[4] ? argv[4] : "active";
    int prepTime = argv[5] ? atoi(argv[5]) : 0;
    string description = argv[6] ? argv[6] : "";
    Address addr("", "", 0, 0);
    Menu menu;
    
    Restaurant r(id, name, addr, prepTime, phone, description, menu);
    restaurants->push_back(r);
    
    return 0;
}

Restaurant* RestaurantDAO::FindById(int id) {
    stringstream sql;
    sql << "SELECT * FROM restaurants WHERE id = " << id << ";";
    vector<Restaurant> results;
    if (db.query(sql.str(), RestaurantCallback, &results)) {
        if (!results.empty()) {
            return new Restaurant(results[0]);
        }
    }
    return nullptr;
}

vector<Restaurant> RestaurantDAO::FindAll() {
    vector<Restaurant> results;
    string sql = "SELECT * FROM restaurants;";
    db.query(sql, RestaurantCallback, &results);
    return results;
}

vector<Restaurant> RestaurantDAO::FindActive() {
    vector<Restaurant> results;
    string sql = "SELECT * FROM restaurants WHERE status = 'active';";
    db.query(sql, RestaurantCallback, &results);
    return results;
}