#ifndef BOOK_H
#define BOOK_H
#include <iostream>
#include <vector>
#include <fstream>

using std::string;
using std::vector;

class Book
{
private:
    string ISBN;
protected:

    string author;
    string title;
    string publisher;
    string genre;
    string description;
    unsigned short year;
    vector <string> keyWords;
    float rating;
    unsigned int id;

    static void performSanityChecks(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, float rating, unsigned short year);
    static void performSanityChecks(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, float rating, unsigned short year, const string& ISBN);

public:
    static unsigned short counter;

    //constructors
    Book();
    Book(const string& author, const string& title,const string& publisher, const string& genre, const string& desc, unsigned short year, float rating);
    Book(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, unsigned short year, float rating, const string& ISBN);
    Book(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, unsigned short year, float rating, const string& ISBN, unsigned short id);

    //operators
    bool operator==(const Book&) const;
    friend std::ostream& operator << (std::ostream& os, const Book&);

    //vars
    static vector<Book*> allItems;
    static unsigned int allItemsCount;

    //getters
    virtual const string& getPersonalNumber();
    const unsigned short getID() {return this->id;}
    vector<string>& getKeyWords() {return this->keyWords;}

    //methods
    virtual Book* clone() const {return new Book(*this);}
    virtual void print(std::ofstream& os) const;
    virtual void displayDetailed();
    void addKeyWord(const string& str);
    static Book* getBookById(unsigned short id);
    static void booksRemove(unsigned short id);
    static void booksAdd(const bool isSeries);


};



#endif //OOP_LIBRARY_PROJECT_BOOK_H
