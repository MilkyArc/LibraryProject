#include <iostream>
#include <cstring>
#include "User.h"
#include "Functionality.h"

using std::cout;
using std::cin;
using std::endl;

Date Functionality::currentDate;
bool Functionality::isLoggedIn = false;
bool Functionality::isAdmin = false;
unsigned int User::userCount = 0;

User* Functionality::currentLoggedUser = nullptr;

unsigned short Book::counter = 0; // for IDs
unsigned int Book::allItemsCount = 0;

vector<User*> User::users;

vector<Book*> Book::allItems;


int main()
{
    Functionality::readItemsFromFile();
    User::readUsersFromFile();

    char command[80];
    do
    {
        cout << ">>>";
        cin.getline(command, 80);
        Functionality::executeCommand(command);
        cout << endl;
    }
    while(strcmp(command, "exit"));

    cout << endl << "Exiting...";
    Functionality::freeMemory();

    return 0;
}



