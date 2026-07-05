#ifndef USER_H
#define USER_H
#include <iostream>
#include "classes.h"
using namespace std;

class CustomerUser;
class MembershipLevel {
    public:
        virtual ~MembershipLevel() {}
        virtual string GetLevelName() const = 0;
        virtual double GetRatio() const = 0;
        virtual double Discount(double totalBasePrice) const = 0;
        virtual double ShippingCost(double baseShipping, double totalOrderPrice) const = 0;
};

class NormalLevel : public MembershipLevel {
    public:
        string GetLevelName() const override { return "Normal"; }
        double GetRatio() const override { return 1.0; }
        double Discount(double BasePrice) const override { return 0.0; }
        double ShippingCost(double BaseShipping, double OrderPrice) const override { return BaseShipping; }
};

class SilverLevel : public MembershipLevel {
    public:
        string GetLevelName() const override { return "Silver"; }
        double GetRatio() const override { return 1.2; }
        double Discount(double BasePrice) const override { return 0.05 * BasePrice; }
        double ShippingCost(double BaseShipping, double OrderPrice) const override { 
            if (OrderPrice > 500000) return BaseShipping * 0.3;
            return BaseShipping;
        }
};

class GoldLevel : public MembershipLevel {
    public:
        string GetLevelName() const override { return "Gold"; }
        double GetRatio() const override { return 1.5; }
        double Discount(double BasePrice) const override { return 0.1 * BasePrice; }
        double ShippingCost(double BaseShipping, double OrderPrice) const override { return BaseShipping * 0.5; }
};

class VIPLevel : public MembershipLevel {
    public:
        string GetLevelName() const override { return "VIP"; }
        double GetRatio() const override { return 2.0; }
        double Discount(double BasePrice) const override { return 0.15 * BasePrice; }
        double ShippingCost(double BaseShipping, double OrderPrice) const override { return 0.0; }
};

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