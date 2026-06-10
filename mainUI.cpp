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
    if (existingRests.empty()){
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

        bool res1 = itemDAO.Insert(food1, 1);
        cout << (res1 ? "SS\n" : "FF\n");
        delete food1;
        delete drink1;
        delete food2;

        cout << "[SUCCESS] Test data successfully injected!" << endl;
    }
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
            Cart cart;
            while(1){    
                system("cls");
                cout << "--- Now You Are Customer ---\n\n";
                cout << "  [1] View Restaurants & Menu\n";
                cout << "  [2] Manage Cart\n";
                cout << "  [3] History\n";
                cout << "  [0] Back To Main Menu\n";
                int cuschoice; cin >> cuschoice;
                if (cuschoice == 0) break;
                else if (cuschoice == 1) {
                    cout << "  Available Restaurants:\n\n";
                    vector <Restaurant> ActiveRests = restDAO.FindActive();
                    if (ActiveRests.size() == 0) {
                        cout << "    There is No Active Restaurants!\n";
                        break;
                    }
                    else {
                        for (int i=0; i<ActiveRests.size(); i++) {
                            cout << "    [ " << ActiveRests[i].GetID() << " ] " << ActiveRests[i].Getname()
                                << " (Prepare Time : " << ActiveRests[i].GetPrep() << "mins)\n";
                        }
                        cout << endl;
                        int restChoice;
                        cout << "  [0] Back to Main Menu\n  Or\n";
                        cout << "  Enter Restaurant ID : "; cin >> restChoice; cout << endl;
                        if (restChoice == 0) break;
                        if (!cart.Empty() && restChoice != cart.GetRestID()) {
                            cout << "You Have Items From Another Restaurant!\n";
                            cout << "Please Finalize Your Order Or Clear Cart!\n";
                            getchar(); getchar();
                            continue;
                        }
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
                            cout << "[1] Add Item To Cart\n";
                            int ordChoice; cin >> ordChoice;
                            if (ordChoice == 0) break;
                            else {
                                cout << "  Enter Item ID To Purchase : "; int q; cin >> q; cout << endl;
                                cout << "  Enter Count : "; int t; cin >> t; cout << endl;
                                Item* ChosenItem = itemDAO.FindById(q);
                                if (t>0 && ChosenItem) {
                                    cart.AddItem(ChosenItem, t, restChoice);
                                    cout << "Item Added Successfully\n";
                                }
                                else {
                                    cout << "Adding Item Failed!\n";
                                    continue;
                                }
                            }
                        }
                    }
                }
                else if (cuschoice == 2) {
                    while(1){
                        system("cls");
                        cart.Display();
                        cout << "  [1] Edit Item Count\n";
                        cout << "  [2] Remove Item\n";
                        cout << "  [3] Finalize Order\n";
                        cout << "  [0] Back\n";
                        int cachoice; cin >> cachoice;
                        if (cachoice == 0) break;
                        else if (cachoice == 2) {
                            system("cls");
                            cart.Display();
                            cout << "[0] Back\nOr\n";
                            cout << "Enter Item Number To Remove : "; int w; cin >> w;
                            if (w == 0) break;
                            else {
                                cart.RemoveItem(w);
                                continue;;
                            }
                        }
                        else if (cachoice == 1) {
                            system("cls");
                            cart.Display();
                            cout << "[0] Back\n";
                            cout << "Enter Item Number To Edit : "; int w; cin >> w; cout << endl;
                            cout << "Enter New Count : "; int e; cin >> e; cout << endl;
                            if (w == 0) break;
                            else {
                                if (e > 0 && itemDAO.FindById(w)){
                                    cart.UpdateCount(w, e);
                                    continue;   
                                }
                                else {
                                    cout << "Invalid Data!\n";
                                    continue;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (choice == 2) {
            while(1) {
                system("cls");
                cout << "--- Now You Are Manager ---\n\n";
                cout << "  Restaurants To Manage : \n";
                vector <Restaurant> AllRest = restDAO.FindAll();
                if (AllRest.size() == 0) {
                    cout << "\tThere Is No Resturants To Manage!\n";
                    break;
                }
                else {
                    for (int i=0; i<AllRest.size(); i++) {
                        cout << "    [ " << AllRest[i].GetID() << " ] " << AllRest[i].Getname() << endl;
                    }
                    cout << endl;
                    int ManageChoice;
                    cout << "  [0] Back to Main Menu\n  Or\n";
                    cout << "  Enter Restaurant ID : "; cin >> ManageChoice; cout << endl;
                    if (ManageChoice == 0) break;
                    Restaurant* SelectedRest = restDAO.FindById(ManageChoice);
                    if (SelectedRest == nullptr) {
                        cout << "\tInvalid ID!! Try Again!\n";
                        continue;
                    }
                    while(1) {
                        system("cls");
                        cout << "=== Your Restuarant Information ===\n\n";
                        cout << "  ID : " << SelectedRest->GetID() << endl;
                        cout << "  Name : " << SelectedRest->Getname() << endl;
                        cout << "  Address : " << SelectedRest->Getaddress();
                        cout << "  Prep_Time : " << SelectedRest->GetPrep() << endl;
                        cout << "  Phone_Number : " << SelectedRest->GetPhone() << endl;
                        cout << "  Description : " << SelectedRest->Getdesc() << endl;
                        cout << "  Status : " << SelectedRest->GetStatus() << endl;
                        cout << "  Menu : \n";
                        vector <Item*> mennu = itemDAO.FindByRestaurant(ManageChoice);
                        for (int i=0; i<mennu.size(); i++) mennu[i]->Display();
                        cout << endl;
                        cout << "  [1] Add Item\n";
                        cout << "  [2] Update Item\n";
                        cout << "  [3] Remove Item\n";
                        cout << "  [4] Edit Info\n";
                        cout << "  [0] Back To Restaurants\n";
                        int cchoice; cin >> cchoice;
                        if (cchoice == 0) break;
                        else if (cchoice == 1 || cchoice == 2) {
                            system("cls");
                            int id, type, prep, activity;
                            Status stat;
                            Type typ;
                            string name, desc;
                            double price, volume;
                            cout << "--- Adding/Updating Item ---\n";
                            cout << "  Enter Item ID : "; cin >> id; cin.ignore(); cout << endl;
                            if (cchoice == 2) itemDAO.FindById(id)->Display();
                            cout << "  Enter Item Name : "; getline(cin, name); cout << endl;
                            cout << "  Enter Item Description : "; getline(cin, desc); cout << endl;
                            cout << "  Enter Item Price : "; cin >> price; cout << endl;
                            cout << "  Enter Item Activity ([1] Active , [0] InActive): "; cin >> activity; cout << endl;
                            (activity == 1) ? stat = Active : stat = InActive;
                            cout << "  Enter Item Type ([1] Food , [2] Drink): "; cin >> type; cout << endl;
                            (type == 1) ? typ = Food : typ = Drink;
                            Item* newItem = nullptr;
                            if (type == 1) {
                                cout << "  Enter Prep_Time : "; cin >> prep; cout << endl;
                                newItem = new FoodC(id, name, desc, price, Food, stat, prep);
                            }
                            else {
                                cout << "  Enter Volume : "; cin >> volume; cout << endl;
                                newItem = new DrinkC(id, name, desc, price, Drink, stat, volume);
                            }
                            if (cchoice == 1) {
                                if (itemDAO.Insert(newItem , ManageChoice)) cout << "Item Added Successfully\n";
                                else cout << "Adding Item Failed!\n";
                            }
                            else {
                                if (itemDAO.Update(newItem)) cout << "Item Updated Successfully\n";
                                else cout << "Updating Item Failed!\n";
                            }
                            delete newItem;
                        }
                        else if (cchoice == 3) {
                            system("cls");
                            cout << "--- Removing Item ---\n";
                            cout << "  Enter Item ID to Remove : "; int id; cin >> id; cout << endl;
                            if(itemDAO.Remove(id)) cout << "Item Removed Successfully\n";
                            else cout << "Removing Item Failed1\n";
                        }
                        else if (cchoice == 4) {
                            while(1) { 
                                system("cls");
                                cout << "=== Your Restuarant Information ===\n\n";
                                cout << "  ID : " << SelectedRest->GetID() << endl;
                                cout << "  Name : " << SelectedRest->Getname() << endl;
                                cout << "  Address : " << SelectedRest->Getaddress();
                                cout << "  Prep_Time : " << SelectedRest->GetPrep() << endl;
                                cout << "  Phone_Number : " << SelectedRest->GetPhone() << endl;
                                cout << "  Description : " << SelectedRest->Getdesc() << endl;
                                cout << "  Status : " << SelectedRest->GetStatus() << endl;
                                cout << "\n--- Editing Restaurant Info ---\n";
                                cout << "  Enter What You Want To Edit :\n";
                                cout << "    [1] ID , [2] Name , [3] Address , [4] Prep_Time \n";
                                cout << "    [5] Phone_Number , [6] Description , [7] Status \n";
                                cout << "    [0] If You Finish Editing!\n ";
                                int edchoice; cin >> edchoice;
                                if (edchoice == 0) break;
                                else if (edchoice == 1) {
                                    system("cls");
                                    cout << "Enter New ID : "; int x; cin >> x;
                                    SelectedRest->SetID(x);
                                    continue;
                                }
                                else if (edchoice == 2) {
                                    system("cls");
                                    cout << "Enter New Name : "; string x; cin >> x;
                                    SelectedRest->Setname(x);
                                    continue;
                                }
                                else if (edchoice == 3) {
                                    system("cls");
                                    cout << "Enter New Address : "; Address x; cin >> x;
                                    SelectedRest->Setaddress(x);
                                    continue;
                                }
                                else if (edchoice == 4) {
                                    system("cls");
                                    cout << "Enter New Prep_Time : "; int x; cin >> x;
                                    SelectedRest->SetPrep(x);
                                    continue;
                                }
                                else if (edchoice == 5) {
                                    system("cls");
                                    cout << "Enter New Phone_Number : "; string x; cin >> x;
                                    SelectedRest->SetPhone(x);
                                    continue;
                                }
                                else if (edchoice == 6) {
                                    system("cls");
                                    cout << "Enter New Description : "; string x; cin.ignore(); getline(cin, x); 
                                    SelectedRest->Setdesc(x);
                                    continue;
                                }
                                else if (edchoice == 7) {
                                    system("cls");
                                    cout << "Enter New Status ([1] Active , [0] InActive): "; int x; cin >> x;
                                    (x == 1) ? SelectedRest->SetStatus(Active) : SelectedRest->SetStatus(InActive);
                                    continue;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (choice == 3) {
            while(1){
                system("cls");
                cout << "--- Now You Are Admin ---\n\n";
                vector <Restaurant> AllRest = restDAO.FindAll();
                cout << "  All Restaurants: \n";
                for (int i=0; i<AllRest.size(); i++) cout << "    [ " << AllRest[i].GetID() << " ] " << AllRest[i].Getname() << endl;
                cout << "  \nChoose What YOu Want To Do : \n";
                cout << "    [1] Add Restaurant , [2] Remove Restaurant\n";
                cout << "    [0] Back To Main Menu\n";
                int achoice; cin >> achoice;
                if (achoice == 0) break;
                else if (achoice == 1) {
                    system("cls");
                    cout << "Enter new Restaurant ID : "; int id; cin >> id; cin.ignore(); cout << endl;
                    cout << "Enter new Restaurant Name : "; string name; cin >> name; cout << endl;
                    cout << "Enter new Restaurant Address : ";
                    cout << "\n City , Street , StreetNo , BuildingNo\n"; Address adrs; cin >> adrs; cout << endl;
                    cout << "Enter new Restaurant Prep_Time : "; int pt; cin >> pt; cout << endl;
                    cout << "Enter new Restaurant Phone_Number : "; string pn; cin >> pn; cout << endl;
                    cout << "Enter new Restaurant Description : "; string desc; cin.ignore(); getline(cin, desc); cout << endl;
                    cout << "Enter new Restaurant Status ([1] Active , [0] InActive): "; int sta; cin >> sta; cout << endl;
                    Status statt; (sta == 1) ? statt = Active : statt = InActive;
                    Menu m(id);
                    Restaurant newRest(id, name, adrs, pt, pn, desc, m, statt);
                    if (restDAO.Insert(newRest)) cout << "\nRestaurant Added Successfully\n";
                    else cout << "Adding Restaurant Failed!\n";
                }
                else if (achoice == 2) {
                    system("cls");
                    cout << "Enter Restaurant ID To Remove : "; int x; cin >> x;
                    if (restDAO.Remove(x)) cout << "  Restaurant Removed Successfully!\n";
                    else cout << "  Removing Restaurant Failed!\n";
                }
            }
        }
    }
}