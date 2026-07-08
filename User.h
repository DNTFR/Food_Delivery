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
        virtual void UpdateState(CustomerUser& customer) = 0;
};

class NormalLevel : public MembershipLevel {
    public:
        string GetLevelName() const override { return "Normal"; }
        double GetRatio() const override { return 1.0; }
        double Discount(double BasePrice) const override { return 0.0; }
        double ShippingCost(double BaseShipping, double OrderPrice) const override { return BaseShipping; }
        void UpdateState(CustomerUser& customer);
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
        void UpdateState(CustomerUser& customer);
};

class GoldLevel : public MembershipLevel {
    public:
        string GetLevelName() const override { return "Gold"; }
        double GetRatio() const override { return 1.5; }
        double Discount(double BasePrice) const override { return 0.1 * BasePrice; }
        double ShippingCost(double BaseShipping, double OrderPrice) const override { return BaseShipping * 0.5; }
        void UpdateState(CustomerUser& customer);
};

class VIPLevel : public MembershipLevel {
    public:
        string GetLevelName() const override { return "VIP"; }
        double GetRatio() const override { return 2.0; }
        double Discount(double BasePrice) const override { return 0.15 * BasePrice; }
        double ShippingCost(double BaseShipping, double OrderPrice) const override { return 0.0; }
        void UpdateState(CustomerUser& customer);
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

class CustomerUser : public User {
    public:
        CustomerUser(int id, string name, string pass, int initPoints);
        ~CustomerUser();
        void ChangeLevel(MembershipLevel* newLevel);
        void AddPoints(double orderPrice);
        void DecreasePoints(double amount);

        int GetPoints();
        string GetLevelName();
        double GetDiscount(double price);
        double GetShipping(double baseShipping, double price);
        void InitialLevel();
    private:
        int points;
        MembershipLevel* Level;
};

#endif
