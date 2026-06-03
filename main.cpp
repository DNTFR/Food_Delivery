#include <iostream>
#include "Database.h"
#include "Classes.h"

using namespace std;

int main() {
    // ۱. ایجاد شیء مدیریت دیتابیس
    DatabaseManager db;

    // ۲. باز کردن دیتابیس (اگر فایل نباشد، ساخته می‌شود)
    if (db.open("test_restaurant.db")) {
        cout << "Successfully opened database." << endl;
    } else {
        cout << "Failed to open database." << endl;
        return -1;
    }

    // ۳. ساخت جدول کاربران به صورت مستقیم
    string createTableSQL = "CREATE TABLE IF NOT EXISTS Users ("
                            "ID INTEGER PRIMARY KEY, "
                            "name TEXT, "
                            "role INTEGER, "
                            "phone TEXT);";
    
    cout << "\n--- Creating Table ---" << endl;
    db.execute(createTableSQL);

    // ۴. درج یک کاربر نمونه برای تست
    // فرض می‌کنیم یک کاربر با این مشخصات داریم
    string insertSQL = "INSERT INTO Users (ID, name, role, phone) "
                       "VALUES (1, 'Sina_Tabouti', 3, '09151234567');";
    string insertSQL2 = "INSERT INTO Users (ID, name, role, phone) "
                    "VALUES (2, 'DNTFR', 1, '09151knl4567');";
    
    cout << "\n--- Inserting Data ---" << endl;
    db.execute(insertSQL);
    db.execute(insertSQL2);

    // ۵. خواندن اطلاعات برای اطمینان از درج صحیح
    // این متد از همان Callback که در Database.cpp اضافه کردی استفاده می‌کند
    cout << "\n--- Reading Data (Select Test) ---" << endl;
    string selectSQL = "SELECT * FROM Users;";
    db.selectWithCallback(selectSQL);

    // ۶. بستن دیتابیس
    db.close();

    cout << "\nTest Finished. Check if you see the user data above!" << endl;

    return 0;
}