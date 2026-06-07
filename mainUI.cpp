#include <iostream>
#include "Classes.h"
#include "DAO.h"
#include "Database.h"
using namespace std;

int main() {
    DatabaseManager db;
    if (!db.open("food_delivery.db")) {
        cerr << "Error Opening Database\n";
        return -1;
    }

    RestaurantDAO restDAO(db);
    MenuItemDAO itemDAO(db);
    int choice;
    while(1){
        system("cls");
        cout << " ===== Food Delivery ===== \n";
        cout << "Login as ? \n";
        cout << "\t[1] Customer\n";
        cout << "\t[2] Manager\n";
        cout << "\t[3] Admin\n";
        cout << "\t[0] Exit\n";
        cin >> choice;
        if (choice == 0) {
            cout << "See You Later\n";
            break;
        }
        else if (choice == 1) {
            while(1){    
                system("cls");
                cout << "\n---Now You Are Customer---\n";
                cout << "Available Restaurants:\n";
                vector <Restaurant> ActiveRests = restDAO.FindActive();
                if (ActiveRests.size() == 0) {
                    cout << "\tThere is No Active Restaurants!\n";
                    break;
                }
                else {
                    for (int i=0; i<ActiveRests.size(); i++) {
                        cout << "\t[ " << ActiveRests[i].GetID() << " ] " << ActiveRests[i].Getname()
                             << "( Prepare Time : " << ActiveRests[i].GetPrep() << "mins )\n";
                    }
                    int restChoice;
                    cout << "[0] Back to Main Menu\n";
                    cout << "Enter Restaurant ID : "; cin >> restChoice;
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
                            cout << "Coming Soon\n";
                        }
                    }
                }
            }
        }
    }
}