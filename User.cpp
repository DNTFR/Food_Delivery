#include "User.h"
#include <iostream>
using namespace std;

void NormalLevel::UpdateState(CustomerUser& customer) {
    int pnt = customer.GetPoints();
    if (pnt >= 700) {
        customer.ChangeLevel(new VIPLevel);
        cout << "=====================\n";
        cout << "You Are A VIP Member!\n";
        cout << "=====================\n";
    }
    else if (pnt >= 300) {
        customer.ChangeLevel(new GoldLevel);
        cout << "======================\n";
        cout << "You Are A Gold Member!\n";
        cout << "======================\n";
    }
    else if (pnt >= 100) {
        customer.ChangeLevel(new SilverLevel);
        cout << "========================\n";
        cout << "You Are A Silver Member!\n";
        cout << "========================\n";
    }
}

void SilverLevel::UpdateState(CustomerUser& customer) {
    int pnt = customer.GetPoints();
    if (pnt >= 700) {
        customer.ChangeLevel(new VIPLevel);
        cout << "=====================\n";
        cout << "You Are A VIP Member!\n";
        cout << "=====================\n";
    }
    else if (pnt >= 300) {
        customer.ChangeLevel(new GoldLevel);
        cout << "======================\n";
        cout << "You Are A Gold Member!\n";
        cout << "======================\n";
    }
    else if (pnt < 100) {
        customer.ChangeLevel(new NormalLevel);
        cout << "========================\n";
        cout << "You Are A Normal Member!\n";
        cout << "========================\n";
    }
}

void GoldLevel::UpdateState(CustomerUser& customer) {
    int pnt = customer.GetPoints();
    if (pnt >= 700) {
        customer.ChangeLevel(new VIPLevel);
        cout << "=====================\n";
        cout << "You Are A VIP Member!\n";
        cout << "=====================\n";
    }
    else if (pnt >= 100 && pnt < 300) {
        customer.ChangeLevel(new SilverLevel);
        cout << "========================\n";
        cout << "You Are A Silver Member!\n";
        cout << "========================\n";
    }
    else if (pnt < 100) {
        customer.ChangeLevel(new NormalLevel);
        cout << "========================\n";
        cout << "You Are A Normal Member!\n";
        cout << "========================\n";
    }
}

void VIPLevel::UpdateState(CustomerUser& customer) {
    int pnt = customer.GetPoints();
    if (pnt >= 300 && pnt < 700) {
        customer.ChangeLevel(new GoldLevel);
        cout << "======================\n";
        cout << "You Are A Gold Member!\n";
        cout << "======================\n";
    }
    else if (pnt >= 100 && pnt < 300) {
        customer.ChangeLevel(new SilverLevel);
        cout << "========================\n";
        cout << "You Are A Silver Member!\n";
        cout << "========================\n";
    }
    else if (pnt < 100) {
        customer.ChangeLevel(new NormalLevel);
        cout << "========================\n";
        cout << "You Are A Normal Member!\n";
        cout << "========================\n";
    }
}

User::User(int id, string name, string pass, Role role)
: ID(id) , Name(name) , Password(pass) , role(role) {}

int User::GetID() { return ID; }
string User::GetPass() { return Password; }
string User::GetName() { return Name; }
Role User::GetRole() { return role; }

bool User::CheckPass(string pass) { return pass == Password; }

CustomerUser::CustomerUser(int id, string name, string pass, int initPoints)
: User(id, name, pass, Customer) , points(initPoints) , Level(nullptr) {
    InitialLevel();
}

CustomerUser::~CustomerUser() {
    if (Level) {
        delete Level;
        Level = nullptr;
    }
}

void CustomerUser::ChangeLevel(MembershipLevel* newLevel) {
    if (Level) delete Level;
    Level = newLevel;
}

void CustomerUser::InitialLevel() {
    if (points >= 700) Level = new VIPLevel();
    else if (points >= 300) Level = new GoldLevel();
    else if (points >= 100) Level = new SilverLevel();
    else Level = new NormalLevel();
}

int CustomerUser::GetPoints() { return points; }
string CustomerUser::GetLevelName() { return Level->GetLevelName(); }
double CustomerUser::GetDiscount(double price) { return Level->Discount(price); }
double CustomerUser::GetShipping(double baseShipping, double price) { return Level->ShippingCost(baseShipping, price); }

void CustomerUser::AddPoints(double orderPrice) {
    int earned = static_cast<int>(orderPrice * 0.0001 * Level->GetRatio());
    points += earned;
    cout << "\nYou earned " << earned << " points! Total points : " << points << endl;
    Level->UpdateState(*this); 
}

void CustomerUser::DecreasePoints(double amount) {
    points -= amount;
    if (points < 0) points = 0;
    cout << "\n" << amount << " points decreased. Total points: " << points << endl;
    Level->UpdateState(*this);
}