#include <iostream>
#include <vector>
#include "Database.h"
#include "Classes.h"
#include "DAO.h"
using namespace std;

struct OrderData {
    int id;
    int restaurantId;
    double totalPrice;
    string status;
};

struct OrderItemInfo {
    int itemId;
    int quantity;
    double price;
};

struct ActivityReport {
    int restId;
    int orderCount;
    double totalSales;
};

int LastIdCallback(void* data, int argc, char** argv, char** azColName) {
    int* lastId = static_cast<int*>(data);
    if (argc > 0 && argv[0]) *lastId = atoi(argv[0]);
    return 0;
}

int FetchAllOrdersCallback(void* data, int argc, char** argv, char** azColName) {
    auto* ordersList = static_cast<vector<OrderData>*>(data);
    if (argc >= 4 && argv[0] && argv[1] && argv[2] && argv[3]) {
        OrderData order;
        order.id = atoi(argv[0]);
        order.restaurantId = atoi(argv[1]);
        order.totalPrice = atof(argv[2]);
        order.status = argv[3];
        ordersList->push_back(order);
    }
    return 0;
}

int OrderDetailsCallback(void* data, int argc, char** argv, char** azColName) {
    auto* itemsList = static_cast<vector<OrderItemInfo>*>(data);
    if (argc >= 3 && argv[0] && argv[1] && argv[2]) {
        OrderItemInfo info;
        info.itemId = atoi(argv[0]);
        info.quantity = atoi(argv[1]);
        info.price = atof(argv[2]);
        itemsList->push_back(info);
    }
    return 0;
}

int FetchRestaurantOrdersCallback(void* data, int argc, char** argv, char** azColName) {
    auto* ordersList = static_cast<vector<OrderData>*>(data);
    if (argc >= 4 && argv[0] && argv[1] && argv[2] && argv[3]) {
        OrderData order;
        order.id = atoi(argv[0]);
        order.restaurantId = atoi(argv[1]);
        order.totalPrice = atof(argv[2]);
        order.status = argv[3];
        ordersList->push_back(order);
    }
    return 0;
}

int SingleValueCallback(void* data, int argc, char** argv, char** azColName) {
    double* value = static_cast<double*>(data);
    if (argc > 0 && argv[0]) *value = atof(argv[0]);
    return 0;
}

int ActivityReportCallback(void* data, int argc, char** argv, char** azColName) {
    auto* reportList = static_cast<vector<ActivityReport>*>(data);
    if (argc >= 3 && argv[0] && argv[1] && argv[2]) {
        ActivityReport rep;
        rep.restId = atoi(argv[0]);
        rep.orderCount = atoi(argv[1]);
        rep.totalSales = atof(argv[2]);
        reportList->push_back(rep);
    }
    return 0;
}

int main() {
    DatabaseManager db;
    if (!db.open("food_delivery.db")) {
        cerr << "Failed to open database!\n";
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

    string createOrdersTable = 
        "CREATE TABLE IF NOT EXISTS orders ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "restaurant_id INTEGER, "
        "total_price REAL, "
        "status TEXT);";

    string createOrderItemsTable = 
        "CREATE TABLE IF NOT EXISTS order_items ("
        "order_id INTEGER, "
        "item_id INTEGER, "
        "quantity INTEGER, "
        "price REAL);";

    db.execute(createOrdersTable);
    db.execute(createOrderItemsTable);
    db.execute(createRestaurantTable);
    db.execute(createMenuTable);
    RestaurantDAO restDAO(db);
    MenuItemDAO itemDAO(db);

    int choice;
    while(1){
        system("cls");
        cout << " =========================\n";
        cout << " ===== Food Delivery ===== \n";
        cout << " =========================\n\n";
        system("color B");
        cout << " Login as ? \n\n";
        cout << "   [1] Customer\n\n";
        cout << "   [2] Manager\n\n";
        cout << "   [3] Admin\n\n";
        cout << "   [0] Exit\n\n";
        cin >> choice;
        if (choice == 0) {
            cout << "See You Later\n";
            break;
        }  
        else if (choice == 1) {
            Cart cart;
            while(1){    
                system("cls");
                system("color 9");
                cout << "--- Customed Dashboard ---\n\n";
                cout << "  [1] View Restaurants & Menu\n\n";
                cout << "  [2] Manage Cart\n\n";
                cout << "  [3] History\n\n";
                cout << "  [0] Back To Main Menu\n\n";
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
                            cout << "Press Any Key To Back\n";
                            getchar(); getchar();
                            continue;
                        }
                        Restaurant* SelectedRest = restDAO.FindById(restChoice);
                        if (SelectedRest == nullptr) {
                            cout << "\tInvalid ID!! Try Again!\n";
                            continue;
                        }
                        while (1) {
                            cout << "===== Menu Of " << SelectedRest->Getname() << " Restaurant ===== \n\n";
                            vector <Item*> MenuRest = itemDAO.FindByRestaurant(restChoice);
                            for (int i=0; i<MenuRest.size(); i++){
                                if (MenuRest[i]->IsActive()) MenuRest[i]->Display();
                            }
                            cout << "\n[0] Back To Restaurants\n";
                            cout << "\n[1] Add Item To Cart\n";
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
                        cout << "\n  [1] Edit Item Count\n\n";
                        cout << "  [2] Remove Item\n\n";
                        cout << "  [3] Finalize Order\n\n";
                        cout << "  [0] Back\n";
                        int cachoice; cin >> cachoice;
                        if (cachoice == 0) break;
                        else if (cachoice == 2) {
                            if (!cart.Empty()) {
                                cart.Display();
                                cout << "[0] Back\nOr\n";
                                cout << "Enter Item Number To Remove : "; int w; cin >> w;
                                if (w == 0) break;
                                else {
                                    cart.RemoveItem(w);
                                    continue;
                                }
                            }
                            else {
                                cout << "There Is Nothing In Your Cart To Remove!\n";
                                cout << "Press Any Key To Back\n";
                                getchar(); getchar();
                                continue;
                            }
                        }
                        else if (cachoice == 1) {
                            if (!cart.Empty()){
                                cart.Display();
                                cout << "[0] Back\n";
                                cout << "Enter Item Number To Edit : "; int w; cin >> w; cout << endl;
                                cout << "Enter New Count : "; int e; cin >> e; cout << endl;
                                if (w == 0) break;
                                else {
                                    cart.UpdateCount(w, e);
                                    continue;   
                                }
                            }
                            else {
                                cout << "There Is Nothing In Your Cart To Edit\n";
                                cout << "Press Any Key To Back\n";
                                getchar(); getchar();
                                continue;
                            }
                        }
                        else if (cachoice == 3) {
                            if (!cart.Empty()){
                                cart.Display();
                                cout << "\n  Are You Sure You Want To Finalize Your Order ? ([Y] Yes , [N] No) ";
                                char c; cin >> c;
                                if (c == 'N') break;
                                else {
                                    int tarRestID = cart.GetRestID();
                                    double finPrice = cart.GetPrice();
                                    string insertOrderSql = "INSERT INTO orders (restaurant_id, total_price, status) VALUES (" 
                                                        + to_string(tarRestID) + ", " 
                                                        + to_string(finPrice) + ", 'Pending');";
                                    if (!db.execute(insertOrderSql)) {
                                        cout << "\nCreating Order Failed!\n";
                                        break;
                                    }
                                    int lastOrderId = 0;
                                    db.query("SELECT last_insert_rowid();", LastIdCallback, &lastOrderId);
                                    bool itemsSavedSuccessfully = true;
                                    auto cartItems = cart.GetItems();
                                    for (size_t i = 0; i < cartItems.size(); i++) {
                                        Item* item = cartItems[i].first;
                                        int qty = cartItems[i].second;
                                        double unitPrice = item->GetPrice();
                                        string insertItemSql = "INSERT INTO order_items (order_id, item_id, quantity, price) VALUES ("
                                                            + to_string(lastOrderId) + ", "
                                                            + to_string(item->GetID()) + ", "
                                                            + to_string(qty) + ", "
                                                            + to_string(unitPrice) + ");";
                                        if (!db.execute(insertItemSql)) itemsSavedSuccessfully = false;
                                    }
                                    if (itemsSavedSuccessfully) {
                                        cout << "\nOrder #" << lastOrderId << " Placed Successfully!\n";
                                        cart.Clear();
                                    } else cout << "\nSaving Items To Cart Failed!\n";
                                    getchar(); getchar(); break;
                                }
                            }
                        }   
                    }
                }
                else if (cuschoice == 3) {
                    system("cls");
                    cout << "--- ALL PREVIOUS ORDERS ---\n\n";
                    vector <OrderData> allOrders;
                    string queryAllOrdersSql = "SELECT id, restaurant_id, total_price, status FROM orders ORDER BY id DESC;";
                    db.query(queryAllOrdersSql, FetchAllOrdersCallback, &allOrders);
                    if (allOrders.empty()) cout << "  You Haven't Placed Any Orders Yet!\n";
                    else {
                        for (size_t i = 0; i < allOrders.size(); i++) {
                            cout << " | ORDER #" << allOrders[i].id 
                                 << " | Restaurant ID: " << allOrders[i].restaurantId
                                 << " | Status: [" << allOrders[i].status << "]\n"
                                 << " (Item Name , Qty , Price , Total)\n\n";
                            vector <OrderItemInfo> details;
                            string queryDetailsSql = "SELECT item_id, quantity, price FROM order_items WHERE order_id = " 
                                                     + to_string(allOrders[i].id) + ";";
                            db.query(queryDetailsSql, OrderDetailsCallback, &details);
                            if (details.empty()) cout << "No item details found for this order.\n";
                            else {
                                for (size_t j = 0; j < details.size(); j++) {
                                    Item* foodItem = itemDAO.FindById(details[j].itemId);
                                    cout << "[ " << j+1 << " ] ";
                                    if (foodItem) {
                                        double rowTotal = details[j].quantity * details[j].price;
                                        cout << foodItem->Getname() << " , " << details[j].quantity << " , "
                                        << details[j].price << "T , "
                                        << rowTotal << " Toman\n";

                                        delete foodItem; 
                                    } else {
                                        cout << "Unknown ID\n";
                                    }
                                }
                            }
                            cout << "\n--- Total Bill For This Order : " << allOrders[i].totalPrice << " Toman ---\n\n";
                        }
                    }
                    cout << "Press any key to Back\n";
                    getchar(); getchar();
                }
            }
        }
        else if (choice == 2) {
            while(1) {
                system("cls");
                system("color 5");
                cout << "--- Manager Dashboard ---\n\n";
                cout << "  Restaurants To Manage : \n\n";
                vector <Restaurant> AllRest = restDAO.FindAll();
                if (AllRest.size() == 0) {
                    cout << "\tThere Is No Resturants To Manage!\n";
                    break;
                }
                else {
                    for (int i=0; i<AllRest.size(); i++) cout << "    [ " << AllRest[i].GetID() << " ] " << AllRest[i].Getname() << "\n\n";
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
                        cout << "  [1] Add Item\n\n";
                        cout << "  [2] Update Item\n\n";
                        cout << "  [3] Remove Item\n\n";
                        cout << "  [4] Edit Info\n\n";
                        cout << "  [5] View/Update Orders\n\n";
                        cout << "  [0] Back To Restaurants\n\n";
                        int cchoice; cin >> cchoice;
                        if (cchoice == 0) break;
                        else if (cchoice == 1 || cchoice == 2) {
                            system("cls");
                            int id, type, prep, activity;
                            Status stat;
                            Type typ;
                            string name, desc;
                            double price, volume;
                            cout << "--- Adding/Updating Item ---\n\n";
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
                                cout << "===== Your Restuarant Information =====\n\n";
                                cout << "  ID : " << SelectedRest->GetID() << endl;
                                cout << "  Name : " << SelectedRest->Getname() << endl;
                                cout << "  Address : " << SelectedRest->Getaddress();
                                cout << "  Prep_Time : " << SelectedRest->GetPrep() << endl;
                                cout << "  Phone_Number : " << SelectedRest->GetPhone() << endl;
                                cout << "  Description : " << SelectedRest->Getdesc() << endl;
                                cout << "  Status : " << SelectedRest->GetStatus() << endl;
                                cout << "\n--- Editing Restaurant Info ---\n\n";
                                cout << "  Enter What You Want To Edit :\n\n";
                                cout << "    [1] ID , [2] Name , [3] Address , [4] Prep_Time \n\n";
                                cout << "    [5] Phone_Number , [6] Description , [7] Status \n\n";
                                cout << "    [0] If You Finish Editing!\n\n ";
                                int edchoice; cin >> edchoice;
                                if (edchoice == 0) break;
                                else if (edchoice == 1) {
                                    cout << "Enter New ID : "; int x; cin >> x;
                                    SelectedRest->SetID(x);
                                    restDAO.Update(*SelectedRest);
                                    continue;
                                }
                                else if (edchoice == 2) {
                                    cout << "Enter New Name : "; string x; cin >> x;
                                    SelectedRest->Setname(x);
                                    restDAO.Update(*SelectedRest);
                                    continue;
                                }
                                else if (edchoice == 3) {
                                    cout << "Enter New Address : "; Address x; cin >> x;
                                    SelectedRest->Setaddress(x);
                                    restDAO.Update(*SelectedRest);
                                    continue;
                                }
                                else if (edchoice == 4) {
                                    cout << "Enter New Prep_Time : "; int x; cin >> x;
                                    SelectedRest->SetPrep(x);
                                    restDAO.Update(*SelectedRest);
                                    continue;
                                }
                                else if (edchoice == 5) {
                                    cout << "Enter New Phone_Number : "; string x; cin >> x;
                                    SelectedRest->SetPhone(x);
                                    restDAO.Update(*SelectedRest);
                                    continue;
                                }
                                else if (edchoice == 6) {
                                    cout << "Enter New Description : "; string x; cin.ignore(); getline(cin, x); 
                                    SelectedRest->Setdesc(x);
                                    restDAO.Update(*SelectedRest);
                                    continue;
                                }
                                else if (edchoice == 7) {
                                    cout << "Enter New Status ([1] Active , [0] InActive): "; int x; cin >> x;
                                    (x == 1) ? SelectedRest->SetStatus(Active) : SelectedRest->SetStatus(InActive);
                                    restDAO.Update(*SelectedRest);
                                    continue;
                                }
                            }
                        }
                        else if (cchoice == 5) {
                            while(1) {
                                system("cls");
                                cout << "--- RECEIVED ORDERS FOR : " << SelectedRest->Getname() << "\n";
                                vector <OrderData> restOrders;
                                string queryRestOrdersSql = "SELECT id, restaurant_id, total_price, status FROM orders WHERE restaurant_id = " 
                                                           + to_string(ManageChoice) + " ORDER BY id DESC;";
                                db.query(queryRestOrdersSql, FetchRestaurantOrdersCallback, &restOrders);
                                if (restOrders.empty()) {
                                    cout << "No orders received yet for your restaurant.\n";
                                    cout << "Press Any Key To Back!\n";
                                    getchar(); getchar();
                                    break; 
                                } 
                                else {
                                    for (size_t i = 0; i < restOrders.size(); i++) {
                                        cout << "ORDER #" << restOrders[i].id << " | Current Status: [" << restOrders[i].status << "]\n";
                                        cout << "(Item Name , Qty , Price , Total)\n\n";
                                        vector<OrderItemInfo> details;
                                        string queryDetailsSql = "SELECT item_id, quantity, price FROM order_items WHERE order_id = " 
                                                                 + to_string(restOrders[i].id) + ";";
                                        db.query(queryDetailsSql, OrderDetailsCallback, &details);
                                        for (size_t j = 0; j < details.size(); j++) {
                                            Item* foodItem = itemDAO.FindById(details[j].itemId);
                                            cout << "[ " << j+1 << " ] ";
                                            if (foodItem) {
                                                double rowTotal = details[j].quantity * details[j].price;
                                                cout << foodItem->Getname() << " , " << details[j].quantity << " , "
                                                << details[j].price << "T , "
                                                << rowTotal << " Toman\n";
                                                delete foodItem; 
                                            } else {
                                                cout << "Unknown ID\n";
                                            }
                                        }
                                    cout << "Total Income From This Order: " << restOrders[i].totalPrice << "Toman\n\n";
                                    }
                                }
                                cout << "\n  [1] Update an Order Status\n\n";
                                cout << "  [0] Back to Manager Menu\n\n";
                                int ordchoice; cin >> ordchoice;
                                if (ordchoice == 0) break;
                                else if (ordchoice == 1) {
                                    cout << "\n    Enter Order ID : ";
                                    int ordID; cin >> ordID;
                                    bool orderExists = false;
                                    for (const auto& o : restOrders) {
                                        if(o.id == ordID) {
                                            orderExists = true;
                                            break;
                                        }
                                    }
                                    if (!orderExists) {
                                        cout << "Invalid Order ID!\n";
                                        getchar(); getchar();
                                        continue;
                                    }
                                    cout << "\n  Select New Status:\n\n";
                                    cout << "    [1] Preparing \n\n";
                                    cout << "    [2] Ready for Delivery \n\n";
                                    cout << "    [3] Delivered\n\n";
                                    int statchoice; cin >> statchoice;
                                    string newStatus = "Pending";
                                    if (statchoice == 1) newStatus = "Preparing";
                                    else if (statchoice == 2) newStatus = "Ready For Delivery";
                                    else if (statchoice == 3) newStatus = "Delivered";
                                    string updateSql = "UPDATE orders SET status = '" + newStatus + "' WHERE id = " + to_string(ordID) + ";";
                                    if (db.execute(updateSql)) {
                                        cout << "\nOrder #" << ordID << " status updated to [" << newStatus << "].\n";
                                    } else cout << "\nFailed To Update Status!\n";
                                    break;
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
                system("color 2");
                cout << "--- Admin Dashboard ---\n\n";
                vector <Restaurant> AllRest = restDAO.FindAll();
                cout << "  All Restaurants: \n\n";
                for (int i=0; i<AllRest.size(); i++) cout << "    [ " << AllRest[i].GetID() << " ] " << AllRest[i].Getname() << endl;
                cout << "  \nChoose What YOu Want To Do : \n\n";
                cout << "    [1] Add Restaurant\n\n";
                cout << "    [2] Remove Restaurant\n\n";
                cout << "    [3] Reports\n\n";
                cout << "    [0] Back To Main Menu\n\n";
                int achoice; cin >> achoice;
                if (achoice == 0) break;
                else if (achoice == 1) {
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
                    cout << "Enter Restaurant ID To Remove : "; int x; cin >> x;
                    if (restDAO.Remove(x)) cout << "  Restaurant Removed Successfully!\n";
                    else cout << "  Removing Restaurant Failed!\n";
                }
                else if (achoice == 3) {
                    cout << "--- REPORTS ---\n";
                    double totalOrders = 0;
                    string countSql = "SELECT COUNT(*) FROM orders;";
                    db.query(countSql, SingleValueCallback, &totalOrders);
                    double totalSales = 0;
                    string salesSql = "SELECT SUM(total_price) FROM orders WHERE status = 'Delivered';";
                    db.query(salesSql, SingleValueCallback, &totalSales);
                    double grossSales = 0;
                    string grossSql = "SELECT SUM(total_price) FROM orders;";
                    db.query(grossSql, SingleValueCallback, &grossSales);
                    cout << " --- Overall Orders ---\n";
                    cout << "  Total Orders Placed:     " << (int)totalOrders << " orders\n";
                    cout << "  Net Revenue (Delivered): " << totalSales << " Toman\n";
                    cout << "  Gross Sales (All Orders): " << grossSales << " Toman\n\n";
                    cout << "--- Restaurant Activity ---\n";
                    cout << "   Rest ID | Total Orders Received | Total Revenue\n";
                    vector <ActivityReport> reports;
                    string activitySql = "SELECT restaurant_id, COUNT(id), SUM(total_price) "
                                         "FROM orders GROUP BY restaurant_id ORDER BY SUM(total_price) DESC;";
                    db.query(activitySql, ActivityReportCallback, &reports);
                    if (reports.empty()) cout << "No restaurant activity!\n";
                    else {
                        for (size_t i = 0; i < reports.size(); i++) {
                            Restaurant* r = restDAO.FindById(reports[i].restId);
                            string restName = r ? r->Getname() : "Unknown";
                            cout << "    [ " << reports[i].restId << " ] " << restName
                                 << "| " << reports[i].orderCount << " orders"
                                 << " | " << reports[i].totalSales << " Toman\n";
                            if(r) delete r;
                        }
                    }
                    cout << "\nPress any key to Back!\n";
                    cin.ignore();
                    cin.get();
                }
            }
        }
    }
}