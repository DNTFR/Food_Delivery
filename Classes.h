#ifndef CLASSES_H
#define CLASSES_H
#include <iostream>
#include <vector>
using namespace std;

enum Role {
    Customer,
    Manager,
    Admin
};

enum Type {
    Food,
    Drink,
    Other
};

enum Status {
    Active,
    InActive
};

class Admin {
    public:
        
    private:

};

class Address {
    public:
        Address();
        Address(string , string , int , int);
        friend istream &operator >> (istream &input, Address &a);
        friend ostream &operator << (ostream &output, const Address &a);

        string GetCity() const;
        string GetStreet() const;
        int GetStreetNo() const;
        int GetBuildingNo() const;

        void SetCity(string c);
        void SetStreet(string s);
        void SetStreetNo(int sn);
        void SetBuildNo(int bn);
    private:    
        string City;
        string Street;
        int StreetNo;
        int BuildingNo;
};

class Item {
    public:
        Item();
        Item(int , string , string , double, Type , Status);

        int GetID() const;
        string Getname() const;
        string Getdesc() const;
        double GetPrice() const;
        Type Gettype() const;
        bool Getstatus() const;
        
        void SetID(int ID);
        void Setname(string name);
        void SetDesc(string etc);
        void SetPrice(double price);
        void Setype(Type type);
        void Setstatus(Status status);
        
        bool IsActive();
        void Activate();
        void DeActivate();

        virtual void Display();
    protected:    
        int ID;
        string name;
        string etc;
        double Price;
        Type type;
        Status status;
};

class Menu {
    public:
        Menu();
        Menu(int ID);

        int GetRestaurantId();
        vector <Item*> GetItems();

        int FindItemIndex(int id);    
        void SetRestaurantId(int restaurantId);
        void AddItem(Item* item);
        void RemoveItem(int itemId);
        void UpdateItem(Item* item); // monde
        
        void DisplayAll() const;
        void displayActive() const;
    private:
        int RestaurantID;
        vector <Item*> MenuItems;
};

class Restaurant {
    public:
        Restaurant();
        Restaurant(int , string , Address , int , string , string , Menu , Status);

        void SetID(int ID);
        void Setname(string name);
        void Setaddress(Address address);
        void SetPrep(int Prep_Time);
        void SetPhone(string PhoneNum);
        void Setdesc(string etc);
        void Setmenu(Menu menu);
        void SetStatus(Status status);

        int GetID() const;
        string Getname() const;
        Address Getaddress() const;
        int GetPrep() const;
        string GetPhone() const;
        string Getdesc() const;
        Menu& GetMenu();
        Status GetStatus() const;
    private:
        int ID;
        string name;
        Address address;
        int Prep_Time;
        string Phone_Number;
        string etc;
        Menu menu;
        Status status;
};

class FoodC : public Item {
    public:
        FoodC();
        FoodC(int , string , string , double, Type , Status , int);

        int GetPrep()const;
        void Display() override;

        void SetPrep(int Prep);
    private:
        int Prep_Time;
};

class DrinkC : public Item {
    public:
        DrinkC();
        DrinkC(int , string , string , double, Type , Status, double);

        double GetVolume() const;
        void Display() override;

        void SetVolume(double Volume);
    private:
        double Volume;
};

ostream& operator << (ostream& output, Type type);
ostream& operator << (ostream& output, Status status);
ostream& operator << (ostream& output, Role role);

#endif