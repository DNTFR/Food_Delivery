#include "DAO.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

RestaurantDAO::RestaurantDAO(DatabaseManager& database) : db(database) {}

int RestaurantDAO::RestaurantCallback(void* data, int argc, char** argv, char** azColName) {
    vector<Restaurant>* restaurants = static_cast<vector<Restaurant>*>(data);
    int id = argv[0] ? atoi(argv[0]) : 0;
    string name = argv[1] ? argv[1] : "";
    string city = argv[2] ? argv[2] : "";
    string street = argv[3] ? argv[3] : "";
    int sNo = argv[4] ? atoi(argv[4]) : 0;
    int bNo = argv[5] ? atoi(argv[5]) : 0;
    Address addr(city, street, sNo, bNo);
    int prepTime = argv[6] ? atoi(argv[6]) : 0;
    string phone = argv[7] ? argv[7] : "";
    string description = argv[8] ? argv[8] : "";
    int statusInt = argv[9] ? atoi(argv[9]) : 1;
    Status status = (statusInt == 1) ? Active : InActive;
    Menu menu; 
    restaurants->push_back(Restaurant(id, name, addr, prepTime, phone, description, menu, status));
    return 0;
}

bool RestaurantDAO::Insert(const Restaurant& r) {
    stringstream sql;
    Address addr = r.Getaddress();
    sql << "INSERT INTO restaurants (id, name, city, street, sNo, bNo, prep_time, phone, description, status) VALUES ("
        << r.GetID() << ", '" << r.Getname() << "', '" 
        << addr.GetCity() << "', '" << addr.GetStreet() << "', " 
        << addr.GetStreetNo() << ", " << addr.GetBuildingNo() << ", "
        << r.GetPrep() << ", '" << r.GetPhone() << "', '" << r.Getdesc() << "', "
        << (r.GetStatus() == Active ? 1 : 0) << ");";
    return db.execute(sql.str());
}

bool RestaurantDAO::Update(const Restaurant& r) {
    std::stringstream sql;
    Address addr = r.Getaddress();
    sql << "UPDATE restaurants SET "
        << "name = '" << r.Getname() << "', "
        << "city = '" << addr.GetCity() << "', "
        << "street = '" << addr.GetStreet() << "', "
        << "sNo = " << addr.GetStreetNo() << ", "
        << "bNo = " << addr.GetBuildingNo() << ", "
        << "prep_time = " << r.GetPrep() << ", "
        << "phone = '" << r.GetPhone() << "', "
        << "description = '" << r.Getdesc() << "', "
        << "status = " << (r.GetStatus() == Active ? 1 : 0) << " "
        << "WHERE id = " << r.GetID() << ";";
    return db.execute(sql.str());
}

bool RestaurantDAO::Remove(int id) {
    std::stringstream sql;
    sql << "DELETE FROM restaurants WHERE id = " << id << ";";
    return db.execute(sql.str()); 
}

Restaurant* RestaurantDAO::FindById(int id) {
    stringstream sql;
    sql << "SELECT * FROM restaurants WHERE id = " << id << ";";
    vector <Restaurant> results;
    if (db.query(sql.str(), RestaurantCallback, &results)) {
        if (!results.empty()) {
            return new Restaurant(results[0]);
        }
    }
    return nullptr;
}

vector <Restaurant> RestaurantDAO::FindAll() {
    vector <Restaurant> list;
    string sql = "SELECT * FROM restaurants;";
    db.query(sql, RestaurantCallback, &list);
    return list;
}

vector <Restaurant> RestaurantDAO::FindActive() {
    vector <Restaurant> list;
    string sql = "SELECT * FROM restaurants WHERE status = 1;";
    db.query(sql, RestaurantCallback, &list);
    return list;
}