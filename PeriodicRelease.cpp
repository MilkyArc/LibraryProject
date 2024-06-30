#include "PeriodicRelease.h"
#include "Functionality.h"
using std::cout;
using std::endl;

PeriodicRelease::PeriodicRelease()
{
    *(Book*) this = Book();
    month = 0;
    edition = 0;
    ISSN = "";
}
//DA SE TURQT PROVERKI
PeriodicRelease::PeriodicRelease(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, unsigned short year, float rating,
unsigned short month, unsigned short edition, vector <Article*> articles)
{
    performSanityChecks(month, edition);

    *(Book*) this = Book(author, title, publisher, genre, desc, year, rating);
    this->month = month; // tuka
    this->edition = edition;
    this->articles = articles;
}

PeriodicRelease::PeriodicRelease(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, unsigned short year, float rating,
unsigned short month, unsigned short edition, vector <Article*> articles, const string& ISSN)
{
    *(Book*) this = Book(author, title, publisher, genre, desc, year, rating);
    this->month = month;
    this->edition = edition;
    this->articles = articles;
    this->ISSN = ISSN;
}

void PeriodicRelease::performSanityChecks(unsigned short month, unsigned short edition)
{
    if (!(month > 0 && month <= 12)) throw "Invalid month. Expected 1-12";
    if (edition == 0 || edition > 1000) throw "Edition can't be zero or is too high. Expected <= 1000";
}

void PeriodicRelease::performSanityChecks(unsigned short month, unsigned short edition, const string& ISSN)
{
    performSanityChecks(month, edition);
    if (ISSN.length() != 8) throw "Invalid ISSN. Must be 8 chars long.";

}

void PeriodicRelease::print(std::ofstream& os) const
{
    os << "Series" << "|" << this->id << "|" << this->title << "|" << this->publisher << "|" << this->genre
       << "|" << this->description << "|" << this->year << "|" << this->month << "|" << this->edition
       << "|" << this->rating << "|" << this->ISSN << "|" << endl;

    os << this->articles.size() << endl;
    for (int i = 0; i < this->articles.size(); i++)
    {
        os << *articles[i] << endl;
    }
}

const string& PeriodicRelease::getPersonalNumber()
{
    return this->ISSN;
}

void PeriodicRelease::displayDetailed()
{
    cout << endl;
    cout << this->title << endl;
    cout << this->publisher << endl;
    cout << this->genre << endl;
    cout << this->description << endl;
    cout << this->year << endl;
    cout << Functionality::monthToString(this->month) << endl;
    cout << "edition:" << this->edition << endl;
    cout << "rating:" << this->rating << endl;
    cout << "ID: " << this->id << endl;
    cout << this->ISSN << endl;

    for (int i = 0; i < this->articles.size(); ++i)
    {
        cout << *this->articles[i] << endl; // otdelna func za article, operator e stranen kato faila ??
    }

}

PeriodicRelease::PeriodicRelease(const string &author, const string &title, const string &publisher,
                                 const string &genre, const string &desc, unsigned short year, float rating,
                                 unsigned short month, unsigned short edition, vector<Article *> articles,
                                 const string &ISBN, unsigned short id)
{
    *(Book*) this = Book(author, title, publisher, genre, desc, year, rating);
    this->month = month;
    this->edition = edition;
    this->articles = articles;
    this->ISSN = ISSN;
    this->id = id;
}
