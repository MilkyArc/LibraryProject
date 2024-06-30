#ifndef OOP_LIBRARY_PROJECT_FUNCTIONALITY_H
#define OOP_LIBRARY_PROJECT_FUNCTIONALITY_H
#include <iostream>
#include <fstream>
#include "Reader.h"
#include "Admin.h"
#include "Book.h"
#include "PeriodicRelease.h"

class Functionality
{
private:
    Functionality();

    static unsigned short getBestId();

    static bool doesContainString(char command[], char string[]);

public:
    static bool isLoggedIn;
    static bool isAdmin;
    static User* currentLoggedUser;
    static Date currentDate;

    //create singleton
    Functionality(const Functionality&) = delete;
    Functionality& operator= (const Functionality&) = delete;
    Functionality& getInstance()
    {
        static Functionality instance;
        return instance;
    }

    //functionalities
    static void login(const string& username, const string& password);
    static bool isAuthorized(const string& username, const string& password);
    static void logout();
    static void addUser(const string& username, const string& password, bool isAdmin);
    static void writeItemsToFile();
    static void readItemsFromFile();

    static void addItem(Book* book);

    static void bookAll();
    static void seriesAll();
    static void listAll();
    static void listInfo(const string& isbn_issn);

    static void displayUserInfo(const string& name); // user find name
    static void findUserByTakenItemID(unsigned short itemID);
    static void findUsersByState(const string& optionString);

    static void help();
    static void booksView();

    static const string monthToString(unsigned short month);

    static void getLine(string& str);
    static int toInt(char str[]);
    static float toFloat (char str[]);
    static void executeCommand(char command[]);

    static void freeMemory();
};

#endif //OOP_LIBRARY_PROJECT_FUNCTIONALITY_H
