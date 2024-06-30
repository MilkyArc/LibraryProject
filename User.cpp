#include <cstring>
#include "User.h"
#include <fstream>
#include "crypto/sha256.h"
#include "Admin.h"
#include "Reader.h"
#include "Functionality.h"
#include "Date.h"
using std::cout;
using std::endl;

void User::writeUsersToFile()
{
    std::ofstream file ("Users.txt");
    if (!file.is_open())
    {
        std::cout << "Error opening file!";
        throw "Error opening file!";
    }

    file << users.size() << endl;

    for (int i = 0; i < users.size(); i++)
    {
         users[i]->print(file);
    }
    file.close();
}

User::User(const string& username, const string& password)
{
    SHA256 sha256;

    this->username = username;
    this->auth = sha256(password + username);
}

void User::addUserToList(User* u1)
{
    users.push_back(u1);
    userCount++;
}

User* User::getUserByName(const string& username)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (username == users[i]->getUsername()) return users[i];
    }
    return nullptr;
}

void User::addUser(const string& username, const string& password, bool isAdmin)
{
    if (Functionality::isLoggedIn && Functionality::isAdmin)
    {
        if (!doesUserExist(username))
        {

            if (isAdmin)
            {
                string email;
                string department;
                cout << "Enter this admin's email address: ";
                Functionality::getLine(email);
                cout << "Enter their department: ";
                Functionality::getLine(department);
                Admin::addAdmin(username, password, email, department);
            }

            else Reader::addReader(username, password);
            cout << endl << "New user " << username << " created successfully! " << "Admin:" << std::boolalpha << isAdmin << endl;
        }
        else
        {
            cout << endl << "Username already taken!" << endl;
        }
    }
    else
    {
        cout << endl << "Only admins can add new users." << endl;
    }
}

void User::removeUser(const string& username)
{
    removeUserFromList(getUserByName(username));
    userCount--;
    writeUsersToFile(); // update record
    cout << "User " << username << " removed successfully." << endl;
}

void User::removeUserFromList(User* u1)
{
    vector<User*>::iterator it1;

    for (int i = 0; i < users.size(); i++)
    {
        if (*users[i] == *u1)
        {
            it1 = users.begin() + i;
            users.erase(it1);
        }
    }
    writeUsersToFile(); // update record
}

bool User::operator==(const User& user) const
{
    return (this->username == user.username);
}

void User::changePassword()
{
    if(!Functionality::isLoggedIn)
    {
        cout << endl << "You must be logged in to use this command!";
        return;
    }

    string password;
    cout << endl << "Enter your current password:";
    Functionality::getLine(password);

    string newPass;
    string newPassRepeat;

    if(Functionality::isAuthorized(this->username, password))
    {
        do
        {
            cout << endl << "Enter your new password:";
            Functionality::getLine(newPass);
            cout << endl << "Repeat the new password:";
            Functionality::getLine(newPassRepeat);
        }

        while(newPass != newPassRepeat);

    }
    else return;

    this->setAuth(newPass);
    writeUsersToFile(); // update record

    cout << endl << "Password for user " << this->username << " changed successfully!";

}

void User::changePassword(const string& username)
{
    if (!getUserByName(username)) cout << endl << "No such user exists.";

    if (Functionality::isAdmin)
    {
        string newPass;
        string newPassRepeat;

        do
        {
            cout << endl << "Enter the new password for " << username << ":";
            Functionality::getLine(newPass);
            cout << endl << "Repeat the new password:";
            Functionality::getLine(newPassRepeat);
        }

        while(newPass != newPassRepeat);
        getUserByName(username)->setAuth(newPass);
        writeUsersToFile(); // update record;
    }
    else
    {
        cout << endl << "You must be an admin to change somebody else's password!";
        return;
    }

    cout << endl << "Password for user " << username << " changed successfully!";
}

void User::setAuth(const string& password)
{
    SHA256 sha256;
    string auth = sha256(password + this->username);
    this->auth = auth;
}

void User::readUsersFromFile()
{
    std::ifstream file ("Users.txt");
    if (!file.is_open())
    {
        std::ofstream file1("Users.txt");
        file1.close();
        Admin::addAdmin("admin", "i<3c++", "admin@abv.bg", "central");
        return;
    }

    file >> userCount;

    char ch;
    file.get(ch); // check

    for (int i = 0; i < userCount; i++)
    {
        char type[10]; //reader or admin
        file.getline(type, 10, '|');

        if(!strcmp(type, "Reader"))
        {
            char username[25];
            file.getline(username, 25, '|');

            char auth[65];
            file.getline(auth, 65, '|');
            file.get(ch);

            unsigned short borrowedItemsCount;
            file >> borrowedItemsCount;
            file.get(ch);

            vector<Book*> borrowedItems;
            for (int j = 0; j < borrowedItemsCount; j++)
            {
                unsigned short id;
                file >> id;
                file.get(ch);

                borrowedItems.push_back(Book::getBookById(id));
            }

            vector<Date*> takenDates;
            for (int j = 0; j < borrowedItemsCount; j++)
            {
                char year[5];
                file.getline(year, 5, '-');

                char month[3];
                file.getline(month, 3, '-');

                char day[3];
                file.getline(day, 3);

                takenDates.push_back(Date::constructDateFromStr(year, month, day));
            }

            vector<Date*> returnDates;
            for (int j = 0; j < borrowedItemsCount; j++)
            {
                char year[5];
                file.getline(year, 5, '-');

                char month[3];
                file.getline(month, 3, '-');

                char day[3];
                file.getline(day, 3);

                returnDates.push_back(Date::constructDateFromStr(year, month, day));
            }

            vector<bool> areRealReturns;
            for (int j = 0; j < borrowedItemsCount; j++)
            {
                bool isRealReturn;
                file >> isRealReturn;
                file.get(ch);

                areRealReturns.push_back(isRealReturn);
            }

            Reader* r1 = new Reader(username, auth, true);
            r1->setBorrowedItems(borrowedItems);
            r1->setTakenDates(takenDates);
            r1->setReturnDates(returnDates);
            r1->setAreRealReturns(areRealReturns);

            users.push_back(r1);
        }

        else // Admin
        {
            char username[25];
            file.getline(username, 25, '|');

            char auth[65];
            file.getline(auth, 65, '|');

            char email[30];
            file.getline(email, 30, '|');

            char department[30];
            file.getline(department, 30, '|');

            file.get(ch); // \n
            file.get(ch); // 0
            file.get(ch); // \n

            Admin* a1 = new Admin (username, auth, true, email, department);
            users.push_back(a1);
        }

    }
    file.close();
}

void User::setUsername(const string& username)
{
    this->username = username;
}

User::User(const string& username, const string& auth, bool fromFile)
{
        this->username = username;
        this->auth = auth;
}



bool User::doesUserExist(const string& username)
{
    for (int i = 0; i < userCount; i++)
    {
        if (users[i]->username == username) return true;
    }

    return false;
}

