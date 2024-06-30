#ifndef OOP_LIBRARY_PROJECT_PERIODICRELEASES_H
#define OOP_LIBRARY_PROJECT_PERIODICRELEASES_H
#include "Book.h"
#include "Article.h"

class PeriodicRelease : public Book
{
private:
    string ISSN;
    unsigned short month;
    unsigned short edition;
    vector <Article*> articles;



public:
    //constructors
    PeriodicRelease();
    PeriodicRelease(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, unsigned short year, float rating,
    unsigned short month, unsigned short edition, vector <Article*> articles);

    PeriodicRelease(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, unsigned short year, float rating,
                    unsigned short month, unsigned short edition, vector <Article*> articles, const string& ISBN);

    PeriodicRelease(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, unsigned short year, float rating,
                    unsigned short month, unsigned short edition, vector <Article*> articles, const string& ISBN, unsigned short id);
    //getters
    const string& getPersonalNumber() override;

    //methods
    static void performSanityChecks(unsigned short month, unsigned short edition);
    static void performSanityChecks(unsigned short month, unsigned short edition, const string& ISSN);
    PeriodicRelease* clone() const override {return new PeriodicRelease(*this);}
    void print(std::ofstream& os) const override;
    void displayDetailed() override;
};


#endif //OOP_LIBRARY_PROJECT_PERIODICRELEASES_H
