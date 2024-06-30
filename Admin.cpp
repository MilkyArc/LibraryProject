#include "Admin.h"
using std::endl;
using std::cout;

Admin::Admin(const string& username, const string& password, const string& email, const string& department) : User(username, password)
{
    this->email = email;
    this->department = department;
}

void Admin::addAdmin(const string& username, const string& password, const string& email, const string& department)
{
    Admin* a1 = new Admin(username, password, email, department);
    addUserToList(a1->clone());
    User::writeUsersToFile();
    delete a1;

}

void Admin::print(std::ofstream& os) const
{
    os << "Admin|" <<this->getUsername() << "|" << this->getAuth() << "|" << this->email<< "|" << this->department << "|" << endl;
    os << '0' << endl;
}

void Admin::takeItem(unsigned short id)
{
    cout << endl << "Only readers can take items.";
}

void Admin::returnItem(unsigned short id)
{
    cout << endl << "Only readers can return items.";
}

Admin::Admin(const string& username, const string& auth, bool fromFile, const string& email, const string& department) : User(username, auth, fromFile)
{
    this-> email = email;
    this->department = department;
}
