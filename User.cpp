#include "User.h"
#include <iostream>
using namespace std;

User::User(int id, string name, string pass, Role role)
: ID(id) , Name(name) , Password(pass) , role(role) {}

int User::GetID() { return ID; }
string User::GetPass() { return Password; }
string User::GetName() { return Name; }
Role User::GetRole() { return role; }
bool User::CheckPass(string pass) { return pass == Password; }