#ifndef OOP_LIBRARY_PROJECT_USER_H
#define OOP_LIBRARY_PROJECT_USER_H
#include <iostream>
#include <vector>
#include "Date.h"
#include "Book.h"

using std::string;
using std::vector;

class User
{
private:


protected:
    static void addUserToList(User* u1);
    static void removeUserFromList(User* u1);

    string auth;
    Date registrationDate;
    Date lastAccessedDate;
    string username;
public:
    User() {}
    User(const string& username, const string& password);
    User(const string& username, const string& auth, bool fromFile);

    static unsigned int userCount;

    virtual User* clone() const = 0;
    virtual void print(std::ofstream& os) const = 0;

    static void writeUsersToFile();
    static void readUsersFromFile();

    static vector<User*> users;
    static User* getUserByName(const string& username);
    static bool doesUserExist(const string& username);
    static void addUser(const string& username, const string& password, bool isAdmin);
    static void removeUser(const string& username);

    void changePassword(); // current user
    static void changePassword(const string& username); // any user, admin only

    virtual void takeItem(unsigned short id) = 0;
    virtual void returnItem(unsigned short id) = 0;

    //getters
    const string& getUsername() const {return this->username;}
    const string& getAuth() const {return this->auth;}
    const Date& getRegistrationDate() const {return this->registrationDate;}
    const Date& getLastAccessedDate() const {return this->lastAccessedDate;}

    //setters
    void setAuth(const string& password);
    void setUsername(const string& username);

    //operators
    bool operator == (const User& user) const;
};
#endif //OOP_LIBRARY_PROJECT_USER_H
