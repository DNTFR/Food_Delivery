#include <iostream>
#include "Database.h"
#include "Classes.h"
#include "DAO.h"

using namespace std;

int main() {
    DatabaseManager db;
    if (!db.open("food_delivery.db")) {
        cerr << "Failed to open database. Exiting..." << endl;
        return -1;
    }

    cout << "\n--- Creating Tables ---" << endl;

    string createRestaurantTable = 
        "CREATE TABLE IF NOT EXISTS restaurants ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "city TEXT, "
        "street TEXT, "
        "sNo INTEGER, "
        "bNo INTEGER, "
        "prep_time INTEGER, "
        "phone TEXT, "
        "description TEXT, "
        "status INTEGER DEFAULT 1);";

    string createMenuTable = 
        "CREATE TABLE IF NOT EXISTS menu_items ("
        "id INTEGER PRIMARY KEY, "
        "restaurant_id INTEGER, "
        "name TEXT, "
        "description TEXT, "
        "price REAL, "
        "type TEXT, "
        "is_available INTEGER, "
        "prep_time INTEGER, "
        "volume REAL);";

    db.execute(createRestaurantTable);
    db.execute(createMenuTable);
    cout << "\n--- Testing RestaurantDAO ---" << endl;
    RestaurantDAO restDAO(db);

    Address addr("Tehran", "Vanak", 12, 4);
    Menu emptyMenu(1);
    Restaurant myRestaurant(2, "Shandiz Luxury", addr, 30, "02188888888", "Best Persian Kebabs", emptyMenu , InActive);

    if (restDAO.Insert(myRestaurant)) {
        cout << "[SUCCESS] Restaurant inserted successfully!" << endl;
    } else {
        cout << "[ERROR] Could not insert restaurant." << endl;
    }

    Restaurant* fetchedRest = restDAO.FindById(1);
    if (fetchedRest != nullptr) {
        cout << "[SUCCESS] Found Restaurant in DB:" << endl;
        cout << "\tName: " << fetchedRest->Getname() << endl;
        cout << "\tPhone: " << fetchedRest->GetPhone() << endl;
        cout << "\tDescription: " << fetchedRest->Getdesc() << endl;
        delete fetchedRest;
    } else {
        cout << "[ERROR] Restaurant with ID 1 not found." << endl;
    }
    
    cout << "\n--- Testing MenuItemDAO ---" << endl;
    MenuItemDAO itemDAO(db);

    Item* pizza = new FoodC(10, "Pepperoni Pizza", "Hot & Spicy", 180.0, Food, Active, 20);
    Item* cola = new DrinkC(11, "Coca Cola", "Cold beverage", 25.0, Drink, Active, 330.0);

    if (itemDAO.Insert(pizza)) cout << "[SUCCESS] Pizza inserted!" << endl;
    if (itemDAO.Insert(cola)) cout << "[SUCCESS] Cola inserted!" << endl;

    cout << "\n--- Displaying Items from DB ---" << endl;
    Item* dbItem1 = itemDAO.FindById(10);
    Item* dbItem2 = itemDAO.FindById(11);

    if (dbItem1) {
        dbItem1->Display();
        delete dbItem1;
    }
    if (dbItem2) {
        dbItem2->Display();
        delete dbItem2;
    }

    delete pizza;
    delete cola;

    db.close();
    return 0;
}