#include "Reader.h"
#include "User.h"
#include "Functionality.h"

using std::endl;
using std::cout;


Reader::Reader(const string& username, const string& password) : User(username, password) {}

void Reader::addReader(const string& username, const string& password)
{
    Reader* r1 = new Reader(username, password);
    addUserToList(r1->clone());
    User::writeUsersToFile();
    delete r1;
}

void Reader::print(std::ofstream& os) const
{
    os << "Reader|"<< this->getUsername() << "|" << this->getAuth() << "|" << endl;
    os << this->borrowedItems.size() << endl;

    for (int i = 0; i < this->borrowedItems.size(); i++)
    {
        os << this->borrowedItems[i]->getID() << endl;
    }

    for (int i = 0; i < this->takenDates.size(); i++)
    {
        os << *this->takenDates[i] << endl;
    }

    for (int i = 0; i < this->returnDates.size(); i++)
    {
        os << *this->returnDates[i] << endl;
    }

    for (int i = 0; i < this->areRealReturns.size(); i++)
    {
        os << this->areRealReturns[i] << endl;
    }
}

void Reader::takeItem(unsigned short id)
{

    if (doesItemExist(id) && isItemAvailable(id))
    {
        unsigned short index = 0;
        Date* now = Date::getCurrentDate();
        if(hasItemEverBeenTakenByUser(id, index))
        {
            this->takenDates[index] = now;

            this->returnDates[index] = ((now->getMonth() == 12) ? new Date (27, 1, now->getYear() + 1) : new Date (27, now->getMonth() + 1, now->getYear()));
            this->areRealReturns[index] = false;
        }

        else
        {
            this->borrowedItems.push_back(Book::getBookById(id));
            this->takenDates.push_back(now);
            this->returnDates.push_back(((now->getMonth() == 12) ? new Date (27, 1, now->getYear() + 1) : new Date (27, now->getMonth() + 1, now->getYear())));
            this->areRealReturns.push_back(false);
        }

        users[getUserIndex()] = this;
        this->writeUsersToFile();
        cout << endl << "You have successfully taken the item with id: " << id << "! Kindly return it on time." << endl;
    }

    else
    {
        cout << endl << "The item with this id is already taken or doesn't exist.";
    }
}

bool Reader::isItemAvailable(unsigned short id)
{
    for (int i = 0; i < User::userCount; i++)
    {
        if (typeid(users[i]) == typeid(Reader))
        {
            for (int j = 0; j < ((Reader*)users[i])->getBorrowedItems().size(); j++)
            {
                if(((Reader*)users[i])->getBorrowedItems()[j]->getID() == id && ((Reader*)users[i])->areRealReturns[j] == false) return false;
            }
        }
    }
    return true;
}

void Reader::returnItem(unsigned short id)
{
    if(Functionality::isLoggedIn && !Functionality::isAdmin)
    {
        unsigned short index = 0;

        if (doesItemExist(id) && isItemTakenByUser(id, index))
        {
            this->returnDates[index] = Date::getCurrentDate();
            this->areRealReturns[index] = true;
            users[getUserIndex()] = this;
            this->writeUsersToFile();
            cout << "You have successfully returned the item with id: " << id << endl;
        }

        else
        {
            cout << endl << "You haven't taken an item with this ID or such doesn't exist." << endl;
        }
    }

    else
    {
        cout << "Only readers can return items." << endl;
    }
}

bool Reader::isItemTakenByUser(unsigned short id, unsigned short &index)
{

    for (int i = 0; i < this->borrowedItems.size(); i++)
    {
        if (this->borrowedItems[i]->getID() == id && this->areRealReturns[i] == false)
        {
            index =  i;
            return true;
        }
    }
    return false;
}

bool Reader::hasItemEverBeenTakenByUser(unsigned short id, unsigned short &index)
{
    for (int i = 0; i < this->borrowedItems.size(); i++)
    {
        if(this->borrowedItems[i] == Book::getBookById(id))
        {
            index = i;
            return true;
        }
    }
    return false;
}

unsigned short Reader::getUserIndex()
{

    for (int i = 0; i < userCount; i++)
    {

         if (typeid(*users[i]) == typeid(Reader) && users[i]->getUsername() == this->username)
        {
            return i;
        }
    }

}

bool Reader::doesItemExist(unsigned short id)
{
    for (int i = 0; i < Book::allItems.size(); i++)
    {
        if (Book::allItems[i]->getID() == id) return true;
    }

    return false;
}

Reader::Reader(const string& username, const string& auth, bool fromFile) : User(username, auth, fromFile) {}

void Reader::viewBooks()
{
    for (int i = 0; i < this->borrowedItems.size(); i++)
    {
        if (!this->areRealReturns[i]) cout << *this->borrowedItems[i] << endl;
    }
}




