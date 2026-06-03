#include "Classes.h"
#include <iostream>
using namespace std;

Address::Address(string c, string s, int sn, int bn) 
: City(c) , Street(s) , StreetNo(sn) , BuildingNo(bn) {}

istream &operator >> (istream &input, Address &a) {
    input >> a.City >> a.Street >> a.StreetNo >> a.BuildingNo;
    return input;
}
ostream &operator << (ostream &output, const Address &a) {
    output << a.City << ", " << a.Street << " " << a.StreetNo << ", No. " << a.BuildingNo << "\n";
    return output;
}

User::User() 
: ID(0) , name("") , role(Customer) , Phone_Number("") {}

User::User(int id, string n, Role r, string p) 
: ID(id) , name(n) , role(r) , Phone_Number(p) {}

void User::SetID(int id) { ID = id; }
void User::Setname(string name) { this->name = name; }
void User::Setrole(Role role) { this->role = role; }
void User::SetPhone(string PhoneNum) { this->Phone_Number = PhoneNum; }

int User::GetID() { return ID; }
string User::Getname() { return name; }
Role User::Getrole() { return role; }
string User::GetPhone() { return Phone_Number; }
