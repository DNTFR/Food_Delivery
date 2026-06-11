#ifndef DAO_H
#define DAO_H
#include "Database.h"
#include "Classes.h"
#include <vector>
using namespace std;

class RestaurantDAO {
    public:
        RestaurantDAO(DatabaseManager& db);
        
        bool Insert(const Restaurant& restaurant);
        bool Update(const Restaurant& restaurant);
        bool Remove(int id);

        Restaurant* FindById(int id);
        vector <Restaurant> FindAll();
        vector <Restaurant> FindActive();
    private:
        DatabaseManager& db;
        static int RestaurantCallback(void* data, int argc, char** argv, char** azColName);
};

class MenuItemDAO {
    public:
        MenuItemDAO(DatabaseManager& db);
        
        bool Insert(Item* item, int RestID);
        bool Update(Item* item);
        bool Remove(int id);

        Item* FindById(int id);
        vector <Item*> FindByRestaurant(int restaurantId);
    private:
        DatabaseManager& db;
        static int ItemCallback(void* data, int argc, char** argv, char** azColName);
};

#endif