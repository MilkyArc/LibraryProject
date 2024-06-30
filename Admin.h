#ifndef OOP_LIBRARY_PROJECT_ADMIN_H
#define OOP_LIBRARY_PROJECT_ADMIN_H
#include "User.h"
#include <iostream>
using std::string;

class Admin : public User
{
private:
    string email;
    string department;
public:
    Admin() {}
    Admin(const string& username, const string& password, const string& email, const string& department);
    Admin(const string& username, const string& auth, bool fromFile, const string& email, const string& department);
    static void addAdmin(const string& username, const string& password, const string& email, const string& department);

    virtual Admin* clone() const override {return new Admin(*this);}
    void print(std::ofstream& os) const override;

    void takeItem(unsigned short id) override;
    void returnItem(unsigned short id) override;

    //getters
    const string& getEmail() {return this->email;}
    const string& getDepartment() {return this->department;}

    //setters
    void setEmail(const string& email) {this->email = email;}
    void setDepartment(const string& department) {this->department = department;}

};


#endif //OOP_LIBRARY_PROJECT_ADMIN_H
