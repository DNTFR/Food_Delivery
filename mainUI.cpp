#include <iostream>
#include <vector>
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
    RestaurantDAO restDAO(db);
    MenuItemDAO itemDAO(db);

    vector<Restaurant> existingRests = restDAO.FindAll();
        cout << "[!] Database is empty. Generating test data..." << endl;

        Address addr1("Tehran", "Vanak", 12, 4);
        Menu menu1(1);
        Restaurant rest1(1, "Shandiz VIP", addr1, 30, "021-8888", "Best Persian Kebab", menu1, Active);

        Address addr2("Mashhad", "Ahmadabad", 10, 2);
        Menu menu2(2);
        Restaurant rest2(2, "Burger Joint", addr2, 15, "051-3333", "Fast Food Center", menu2, InActive);
        
        Address addr3("Mashhad", "Koohsangi", 10, 2);
        Menu menu3(3);
        Restaurant rest3(3, "Layali", addr3, 20, "051-3789", "Fast Food Center", menu3, Active);

        restDAO.Insert(rest1);
        restDAO.Insert(rest2);
        restDAO.Insert(rest3);

        Item* food1 = new FoodC(101, "Shishlik Kebab", "Premium meat", 350.0, Food, Active, 35);
        Item* drink1 = new DrinkC(102, "Doogh", "Traditional drink", 25.0, Drink, Active, 500.0);
        
        Item* food2 = new FoodC(103, "Double Cheeseburger", "Juicy beef", 180.0, Food, Active, 20);

        itemDAO.Insert(food1, 1);
        itemDAO.Insert(drink1, 1);
        itemDAO.Insert(food2, 3);

        delete food1;
        delete drink1;
        delete food2;

        cout << "[SUCCESS] Test data successfully injected!" << endl;

    int choice;
    while(1){
        system("cls");
        cout << " ===== Food Delivery ===== \n";
        cout << "Login as ? \n";
        cout << "  [1] Customer\n";
        cout << "  [2] Manager\n";
        cout << "  [3] Admin\n";
        cout << "  [0] Exit\n";
        cin >> choice;
        if (choice == 0) {
            cout << "See You Later\n";
            break;
        }
        else if (choice == 1) {
            while(1){    
                system("cls");
                cout << "--- Now You Are Customer ---\n\n";
                cout << "  Available Restaurants:\n\n";
                vector <Restaurant> ActiveRests = restDAO.FindActive();
                if (ActiveRests.size() == 0) {
                    cout << "\tThere is No Active Restaurants!\n";
                    break;
                }
                else {
                    for (int i=0; i<ActiveRests.size(); i++) {
                        cout << "\t[ " << ActiveRests[i].GetID() << " ] " << ActiveRests[i].Getname()
                             << " (Prepare Time : " << ActiveRests[i].GetPrep() << "mins)\n";
                    }
                    cout << endl;
                    int restChoice;
                    cout << "  [0] Back to Main Menu\n  Or\n";
                    cout << "  Enter Restaurant ID : "; cin >> restChoice; cout << endl;
                    if (restChoice == 0) break;
                    Restaurant* SelectedRest = restDAO.FindById(restChoice);
                    if (SelectedRest == nullptr) {
                        cout << "\tInvalid ID!! Try Again!\n";
                        continue;
                    }
                    while (1) {
                        cout << "===== Menu Of " << SelectedRest->Getname() << " Restaurant ===== \n";
                        vector <Item*> MenuRest = itemDAO.FindByRestaurant(restChoice);
                        for (int i=0; i<MenuRest.size(); i++){
                            MenuRest[i]->Display();
                        }
                        cout << "\n[0] Back To Restaurants\n";
                        cout << "[1] Order Items\n";
                        int ordChoice; cin >> ordChoice;
                        if (ordChoice == 0) break;
                        else {
                            system("cls");
                            cout << "Coming Soon\n";
                        }
                    }
                }
            }
        }
    }
}