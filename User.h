#ifndef USER_H
#define USER_H
#include <iostream>
#include "classes.h"
using namespace std;

class User {
    public:
        User(int id, string name, string pass, Role role);
        virtual ~User() {}

        int GetID();
        string GetName();
        string GetPass();
        Role GetRole();
        bool CheckPass(string pass);
    protected:
        int ID;
        string Name;
        string Password;
        Role role;
};

#endif