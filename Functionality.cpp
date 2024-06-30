#include <iostream>
#include "Functionality.h"
#include "crypto/sha256.h"
#include <cstring>
#include <cmath>
#include <exception>

using std::cin;
using std::cout;
using std::endl;

void Functionality::login(const string& username, const string& password)
{
    if(isLoggedIn)
    {
        cout << "You are already logged in." << endl;
        return;
    }

    if (isAuthorized(username, password))
    {
        isLoggedIn = true;
        currentLoggedUser = User::getUserByName(username)->clone();
        isAdmin = (typeid(*currentLoggedUser) == typeid(Admin));
        cout << "Welcome, " << username << "!" << endl;
    }
}

bool Functionality::isAuthorized(const string& username, const string& password)
{
    SHA256 sha256;
    string auth = sha256(password + username);

    for (int i = 0; i < User::users.size(); i++)
    {
        if (username == User::users[i]->getUsername())
        {
            if (auth == User::users[i]->getAuth()) return true;
            else
            {
                cout << endl << "Access denied. Invalid password." << endl;
                return false;
            }

        }
    }

    cout << "No such user found." << endl;
    return false;
}

void Functionality::logout()
{
    if(!isLoggedIn)
    {
        cout << "You are already logged out." << endl;
        return;
    }

    isLoggedIn = false;
    isAdmin = false;
    currentLoggedUser = nullptr;

    cout << endl << "You have been logged out." << endl;
}

void Functionality::writeItemsToFile()
{
    std::ofstream file ("Items.txt");
    if(!file.is_open())
    {
        cout << "Error opening file!";
        throw "Error opening file!";
    }

    file << Book::allItems.size() << endl;
    for (int i = 0; i < Book::allItems.size(); i++)
    {
        Book::allItems[i]->print(file);
    }

    file.close();
}


void Functionality::addItem(Book* book)
{
    Book::allItems.push_back(book->clone());
    Book::allItemsCount++;

    writeItemsToFile();
}

void Functionality::freeMemory()
{
    for (int i = 0; i < Book::allItems.size(); i++)
    {
        delete Book::allItems[i];
        Book::allItems[i] = nullptr;
    }

    for (int i = 0; i < User::users.size(); i++)
    {
        delete User::users[i];
        User::users[i] = nullptr;
    }

}

void Functionality::listAll()
{
    if(!isLoggedIn)
    {
        cout << "You must be logged in to use this command." << endl;
        return;
    }

    for (int i = 0; i < Book::allItems.size(); ++i)
    {
        cout << *Book::allItems[i] << endl;
    }
}

void Functionality::bookAll()
{
    if(!isLoggedIn)
    {
        cout << "You must be logged in to use this command." << endl;
        return;
    }

    for (int i = 0; i < Book::allItems.size(); i++)
    {
        if(typeid(*Book::allItems[i]) == typeid(Book))  cout << *Book::allItems[i] << endl;
    }
}

void Functionality::seriesAll()
{
    if(!isLoggedIn)
    {
        cout << "You must be logged in to use this command." << endl;
        return;
    }

    for (int i = 0; i < Book::allItems.size(); i++)
    {
        if(typeid(*Book::allItems[i]) == typeid(PeriodicRelease))  cout << *Book::allItems[i] << endl;
    }
}

void Functionality::listInfo(const string& isbn_issn)
{
    //validate isbn / issn

    for (int i = 0; i < Book::allItems.size(); i++)
    {
        if (Book::allItems[i]->getPersonalNumber() == isbn_issn)
        {
            Book::allItems[i]->displayDetailed();
            return;
        }

    }

    cout << endl << "No items match the provided info." << endl;
}




const string Functionality::monthToString(unsigned short month)
{
    switch (month)
    {
        case 1:
            return "January";
            break;
        case 2:
            return "February";
            break;
        case 3:
            return "March";
            break;
        case 4:
            return "April";
            break;
        case 5:
            return "May";
            break;
        case 6:
            return "June";
            break;
        case 7:
            return "July";
            break;
        case 8:
            return "August";
            break;
        case 9:
            return "September";
            break;
        case 10:
            return "October";
            break;
        case 11:
            return "November";
            break;
        case 12:
            return "December";
            break;
        default:
            return "";
            break;
    }
}

void Functionality::getLine(string& str)
{
    char buffer[256];
    cin.getline(buffer, 256);
    str = buffer;

}

void Functionality::displayUserInfo(const string& name)
{
    if (!isAdmin)
    {
        cout << "You must be an admin to use this command!";
        return;
    }

    if(!User::getUserByName(name))
    {
        cout << "No such user found.";
        return;
    }

    cout << endl << User::getUserByName(name)->getUsername();
    cout << endl << User::getUserByName(name)->getAuth();
    cout << endl << "Registered on: " << User::getUserByName(name)->getRegistrationDate();
    cout << endl << "Last accessed on: " << User::getUserByName(name)->getLastAccessedDate();

    if(typeid(User::getUserByName(name)) == typeid(Admin))
    {
        cout << ((Admin*)User::getUserByName(name))->getDepartment();
        cout << ((Admin*)User::getUserByName(name))->getEmail();
    }
    else
    {
        for (int i = 0; i < ((Reader*)User::getUserByName(name))->getBorrowedItems().size(); i++)
        {
            cout << ((Reader*)User::getUserByName(name))->getBorrowedItems()[i];
            cout << ", taken on: " << ((Reader*)User::getUserByName(name))->getBorrowedItemsTakenDates()[i] << endl;
            cout << "Return date: " << ((Reader*)User::getUserByName(name))->getBorrowedItemsReturnDates()[i]
                 << " returned: " << std::boolalpha << ((Reader*)User::getUserByName(name))->getAreRealReturns()[i] << endl;
        }

    }

}

void Functionality::findUserByTakenItemID(unsigned short itemID)
{
    if (!isAdmin)
    {
        cout << "You must be an admin to use this command!";
        return;
    }

    for (int i = 0; i < User::users.size() ; i++)
    {
        if (typeid(*User::users[i]) == typeid(Reader))
        {

            for (int j = 0; j < ((Reader*) User::users[i])->getBorrowedItems().size(); j++)
            {
                if (((Reader *) User::users[i])->getBorrowedItems()[j]->getID() == itemID && !((Reader *)User::users[i])->getAreRealReturns()[j])
                {
                    cout << endl << ((Reader*) User::users[i])->getUsername() << endl;
                    return;
                }
            }
        }
    }
    cout << endl << "No user has taken the item with id:" << itemID << " or no item with such id exists." << endl;
}

void Functionality::findUsersByState(const string& optionString)
{
    if (!isAdmin)
    {
        cout << "You must be an admin to use this command!";
        return;
    }

    if (optionString != "overdue" && optionString != "reader" && optionString != "inactive")
    {
        cout << "Invalid option string. Use 'reader, 'overdue' or 'inactive'.";
        return;
    }

    if (optionString == "overdue")
    {
        bool areThereOverdueUsers = false;

        for (int i = 0; i < User::users.size(); i++)
        {
            if (typeid(*User::users[i]) == typeid(Reader))
            {
                for (int j = 0; j < ((Reader *) (User::users[i]))->getBorrowedItemsReturnDates().size(); j++)
                {
                    if ((*((Reader *) (User::users[i]))->getBorrowedItemsReturnDates()[j]) < currentDate
                        && !((Reader *) User::users[i])->getAreRealReturns()[j])
                    {
                        areThereOverdueUsers = true;
                        cout << endl << User::users[i]->getUsername();
                        break;
                    }

                }
            }

        }

        if (!areThereOverdueUsers) cout << endl << "No overdue users found.";
        return;
    }

    else if (optionString == "reader")
    {
        bool hasFoundAnyActiveReaders = false;
        Date monthBehind;
        if(currentDate.getMonth() == 1) monthBehind = Date(currentDate.getDay(), 12, currentDate.getYear() - 1);
        else monthBehind = Date(currentDate.getDay(), currentDate.getMonth() - 1, currentDate.getYear());

        unsigned short counter = 0;
        for (int i = 0; i < User::users.size(); i++)
        {
            if (typeid(*User::users[i]) == typeid(Reader))
            {
                for (int j = 0; j < ((Reader *) User::users[i])->getBorrowedItems().size(); j++)
                {
                    if (monthBehind < (*((Reader *) (User::users[i]))->getBorrowedItemsTakenDates()[j]))
                    {
                        counter++;
                        if (counter == 6)
                        {
                            cout << endl << User::users[i]->getUsername();
                            counter = 0;
                            hasFoundAnyActiveReaders = true;
                            break;
                        }
                    }
                }
            }
        }

        if (!hasFoundAnyActiveReaders) cout << endl << "No active users found.";
    }

    else if (optionString == "inactive")
    {
        bool isInactive = true;
        bool hasFoundAnyInactiveUsers = false;
        Date threeMonthsBehind;
        if(currentDate.getMonth() <= 3) threeMonthsBehind = Date(currentDate.getDay(), 12 - (3 - currentDate.getMonth()), currentDate.getYear() - 1);
        else threeMonthsBehind = Date(currentDate.getDay(), currentDate.getMonth() - 3, currentDate.getYear());

        unsigned short counter = 0;
        for (int i = 0; i < User::users.size(); i++)
        {
            if (typeid(*User::users[i]) == typeid(Reader))
            {
                isInactive = true;
                for (int j = 0; j < ((Reader *) User::users[i])->getBorrowedItems().size(); j++)
                {
                    if (threeMonthsBehind < (*((Reader *) (User::users[i]))->getBorrowedItemsTakenDates()[j]))
                    {
                        isInactive = false;
                        break;
                    }
                }
                if (isInactive)
                {
                    hasFoundAnyInactiveUsers = true;
                    cout << endl << User::users[i]->getUsername();
                }
            }

            if (!hasFoundAnyInactiveUsers) cout << endl << "No inactive users found.";
        }

    }

}

void Functionality::readItemsFromFile()
{
    std::ifstream file ("Items.txt");
    if (!file.is_open())
    {
        std::ofstream file1("Items.txt");
        file1 << 0;
        file1.close();
        return;
    }

    file >> Book::allItemsCount;

    char ch;
    file.get(ch);

    for (int i = 0; i < Book::allItemsCount; i++)
    {

        char type[10]; //book or series
        file.getline(type, 10, '|');

        char id[4];
        file.getline(id, 4, '|');

        if (!strcmp(type, "Book"))
        {

            char author[40];
            file.getline(author, 40, '|');

            char title[40];
            file.getline(title, 40, '|');

            char publisher[40];
            file.getline(publisher, 40, '|');

            char genre[40];
            file.getline(genre, 40, '|');

            char desc[150];
            file.getline(desc, 150, '|');

            char year[5];
            file.getline(year, 5, '|');

            char rating[4];
            file.getline(rating, 4, '|');

            char isbn[15];
            file.getline(isbn, 15);

            unsigned short keyWordsCount;
            file >> keyWordsCount;
            file.get(ch);
            vector<string> keyWords;
            for (int j = 0; j < keyWordsCount; j++)
            {
                char _keyWord[40];
                file.getline(_keyWord, 40);
                string keyWord = _keyWord;
                keyWords.push_back(keyWord);
            }

            Book* b1 = new Book(author, title, publisher, genre, desc, toInt(year), toFloat(rating), isbn, toInt(id));
            for (int j = 0; j < keyWordsCount; j++)
            {
                b1->getKeyWords().push_back(keyWords[j]);
            }

            Book::allItems.push_back(b1);
        }

        else
        {
            char title[30];
            file.getline(title, 30, '|');

            char publisher[40];
            file.getline(publisher, 40, '|');

            char genre[40];
            file.getline(genre, 40, '|');

            char desc[60];
            file.getline(desc, 60, '|');

            char year[5];
            file.getline(year, 5, '|');

            char month[3];
            file.getline(month, 3, '|');

            char edition[4];
            file.getline(edition, 4, '|');

            char rating[5];
            file.getline(rating, 5, '|');
            int debug = toFloat(rating);

            char ISSN[15];
            file.getline(ISSN, 15, '|');

            unsigned short articleCount;
            file >> articleCount;
            file.get(ch);
            vector<Article*> articles;

            for (int j = 0; j < articleCount; j++)
            {
                char _Title[40]; // nashiq getLine() ?
                file.getline(_Title, 40, '|');

                char _Author[30];
                file.getline(_Author, 30, '|');

                unsigned short keyWordCount;
                file >> keyWordCount;
                file.get(ch);
                vector<string> keyWords;

                for (int k = 0; k < keyWordCount; k++)
                {
                    char _keyWord[40];
                    file.getline(_keyWord, 40);
                    string keyWord = _keyWord;
                    keyWords.push_back(keyWord);
                }

                Article* art = new Article(_Title, _Author, keyWords);
                articles.push_back(art);
            }

            PeriodicRelease* s1 = new PeriodicRelease("", title, publisher, genre, desc, toInt(year), toFloat(rating),toInt(month), toInt(edition), articles, ISSN, toInt(id));
            Book::allItems.push_back(s1);
        }
    }
    Book::counter = getBestId();

}

int Functionality::toInt(char str[])
{
    int number = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        number += (int)(str[i] - '0') * pow(10, strlen(str) - i - 1);
    }

    return number;
}

float Functionality::toFloat(char str[])
{
    float number = 0;
    bool hasDecimalPassed = false;

    int digitsBeforeDecimal = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '.') break;
        digitsBeforeDecimal++;

    }


    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '.')
        {
            hasDecimalPassed = true;
            i++;
        }

        if (!hasDecimalPassed)
        {
            number += (int)(str[i] - '0') * pow(10, digitsBeforeDecimal - i - 1);
        }

        else
        {
            number += (int)(str[i] - '0') * pow(10, (digitsBeforeDecimal + 1) - i - 1);
        }
    }

    return number;
}

unsigned short Functionality::getBestId()
{
    unsigned short max = 0;
    if(Book::allItems.size() != 0) max = Book::allItems[0]->getID();
    else return 0;

    for (int i = 0; i < Book::allItems.size(); i++)
    {
        if (max < Book::allItems[i]->getID())
        {
            max = Book::allItems[i]->getID();
        }
    }

    return max;
}

void Functionality::help()
{
    cout << endl << "Here are the available commands and their syntax:" << endl;

    cout << "$ help --> Brings up this informational screen." << endl;
    cout << "$ login <username> <password> --> logs you in." << endl;
    cout << "$ logout --> logs you out." << endl;
    cout << "$ books all --> Displays info about all available books in the library." << endl;
    cout << "$ books view --> Shows the logged user's currently taken items." << endl;
    cout << "$ series all --> Displays info about all available series in the library." << endl;
    cout << "$ list all --> Displays info about all available items in the library." << endl;
    cout << "$ list info <isbn_value> --> Displays detailed info about an item with the corresponding ISBN/ISSN value. " << endl << endl;
    cout << "$ user find <option> <option_string> --> returns a list of users who meet the set criteria;" << endl;
    cout << "<option> can be either <name>, <ID> or <state>." << endl;
    cout << "$ user find name <username> --> Displays info about the user.";
    cout << "$ user find id <id> --> returns the user who has taken the item with that id." << endl;
    cout << "The option string for <state> can be either <overdue>, <reader> or <inactive>" << endl;
    cout << "$ user find state overdue --> Returns all users who haven't returned items in time." << endl;
    cout << "$ user find state reader --> Returns all users who have taken more than 5 books in the last month" << endl;
    cout << "$ user find state inactive --> Returns all users who haven't taken any items for more than 3 months." << endl << endl;
    cout << "$ user add <username> <password> [admin] --> Registers a new user, who can also be an admin." << endl;
    cout << "$ user remove <username> --> Deletes the user from the system. " << endl;
    cout << "$ user change [<username>] --> Changes the current user's password. Admins can provide a third parameter to change the password of other users." << endl;
    cout << "$ take <id> --> Take an available item from the library." << endl;
    cout << "$ return <id> --> Return an item you have taken." << endl;
    cout << "$ exit --> Terminates the program." << endl << endl;

}

void Functionality::booksView()
{
    ((Reader*)currentLoggedUser)->viewBooks();
}

void Functionality::executeCommand(char command[])
{
    if(!strcmp(command, "help"))
    {
        help();
    }
    else if(!strcmp(command, "login"))
    {
        cout << "Enter username: ";
        char username[40];
        cin.getline(username, 40);

        cout << "Enter password: ";
        char password[40];
        cin.getline(password, 40);

        Functionality::login(username, password);
    }
    else if(!strcmp(command, "logout"))
    {
        logout();
    }
    else if(!strcmp(command, "books all"))
    {
        bookAll();
    }
    else if(!strcmp(command, "books view"))
    {
        if(!isLoggedIn || isAdmin)
        {
            cout << "You must be logged in to view your books." << endl;
            return;
        }

        booksView();
    }

    else if (!strcmp(command, "books add"))
    {
        if(!isLoggedIn || !isAdmin)
        {
            cout << "Only admins can add books!" << endl;
            return;
        }

        string choice;
        do
        {
            cout << "Do you want it to be series (y/n)?" << endl;
            cin >> choice;
        }
       while (choice != "n" && choice != "y");
       cin.ignore();

       if(choice == "y")
       {
           Book::booksAdd(true);
       }
       else
       {
           Book::booksAdd(false);
       }

    }

    else if (doesContainString(command,"books remove"))
    {
        char id[20];

        for (int i = 13; i < strlen(command); i++)
        {
            id[i - 13] = command[i];
            if(i == strlen(command) - 1) id[i - 13 + 1] = '\0';
        }

        Book::booksRemove(Functionality::toInt(id));
    }

    else if(!strcmp(command, "series all"))
    {
        seriesAll();
    }
    else if(!strcmp(command, "list all"))
    {
        listAll();
    }
    else if(doesContainString(command, "list info"))
    {
        char isbn[14];
        for (int i = 10; i < strlen(command); i++)
        {
            isbn[i - 10] = command[i];
        }
        isbn[strlen(command) - 10] = '\0';
        listInfo(isbn);
    }

    else if(doesContainString(command, "user add"))
    {
        char username[40], password[40], adminTrigger[40];
        unsigned short spaceCounter = 0;
        unsigned short iter = 0;
        unsigned short passLen = 0;
        adminTrigger[0] = '\0';

        for (int i = 9; i < strlen(command); i++)
        {
            if (command[i] == ' ')
            {
                switch(spaceCounter)
                {
                    case 0:
                        username[iter] = '\0';
                        break;

                    case 1:
                        password[iter] = '\0';
                        break;
                }

                spaceCounter++;
                iter = 0;
                i++;
            }
            if(spaceCounter == 0)
            {
                username[iter] = command[i];
                iter++;
            }
            else if(spaceCounter == 1)
            {
                passLen++;
                password[iter] = command[i];
                iter++;
            }
            else if(spaceCounter == 2)
            {
                adminTrigger[iter] = command[i];
                iter++;
            }
        }
        password[passLen] = '\0';
        adminTrigger[iter] = '\0';

        if (!strcmp(adminTrigger, "admin"))
        {
            User::addUser(username, password, true);
        }
        else if (!strcmp(adminTrigger, ""))
        {
            User::addUser(username, password, false);
        }
        else
        {
            cout << "Invalid third parameter. Did you mean 'admin'?";
        }
    }
    else if (doesContainString(command, "user remove"))
    {
        char username[40];
        for (int i = 12; i < strlen(command); i++)
        {
            username[i - 12] = command[i];
            if(i == strlen(command) - 1) username[i - 12 + 1] = '\0';
        }
        User::removeUser(username);
    }
    else if (doesContainString(command, "user change"))
    {
        if (!isLoggedIn)
        {
            cout << "You must be logged in." << endl;
            return;
        }
        char username[40];
        username[0] = '\0';

        for (int i = 12; i < strlen(command); i++)
        {
            username[i - 12] = command[i];
            if(i == strlen(command) - 1) username[i - 12 + 1] = '\0';
        }
        if (!strcmp(username, ""))
        {
            currentLoggedUser->changePassword();
        }
        else
        {
            currentLoggedUser->changePassword(username);
        }

    }
    else if (doesContainString(command, "take"))
    {
        if(!isLoggedIn || isAdmin)
        {
            cout << "A reader must be logged in to take items." << endl;
            return;
        }

        char id[20];

        for (int i = 5; i < strlen(command); i++)
        {
            id[i - 5] = command[i];
            if(i == strlen(command) - 1) id[i - 5 + 1] = '\0';
        }
         currentLoggedUser->takeItem(toInt(id));
    }

    else if (doesContainString(command, "return"))
    {
        if(!isLoggedIn || isAdmin)
        {
            cout << "A reader must be logged in to return items." << endl;
            return;
        }

        char id[20];

        for (int i = 7; i < strlen(command); i++)
        {
            id[i - 7] = command[i];
            if(i == strlen(command) - 1) id[i - 7 + 1] = '\0';
        }
        currentLoggedUser->returnItem(toInt(id));
    }



    else if(!strcmp(command, "exit"))
    {
        //Exit is a valid command
    }
    else
    {
        cout << "Invalid command!";
    }
}

bool Functionality::doesContainString(char command[], char string[])
{
    for (int i = 0; i < strlen(command); ++i)
    {
        unsigned short counter = 0;
        if(i <= strlen(command) - strlen(string))
        {
            for (int j = 0; j < strlen(string); ++j)
            {
                if(string[j] == command[i + j]) counter++;
                else break;
            }
        }

        if(counter == strlen(string)) return true;
    }
    return false;
}



