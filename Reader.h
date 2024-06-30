#ifndef OOP_LIBRARY_PROJECT_READER_H
#define OOP_LIBRARY_PROJECT_READER_H
#include "User.h"

class Reader : public User
{
private:
    vector <Book*> borrowedItems;
    vector <Date*> takenDates;
    vector <Date*> returnDates;
    vector <bool> areRealReturns;


    unsigned short getUserIndex();

public:
    Reader() {}
    Reader(const string& username, const string& password);
    Reader(const string& username, const string& auth, bool fromFile);

    static void addReader(const string& username, const string& password);

    virtual Reader* clone() const override {return new Reader(*this);}
    virtual void print(std::ofstream& os) const override;

    void takeItem(unsigned short id) override;
    void returnItem (unsigned short id) override;
    void viewBooks();

    static bool isItemAvailable(unsigned short id);
    bool isItemTakenByUser(unsigned short id, unsigned short &index);
    bool hasItemEverBeenTakenByUser(unsigned short id, unsigned short &index);
    static bool doesItemExist(unsigned short id);

    //getters
    const vector<Book*> getBorrowedItems() const {return this->borrowedItems;}
    const vector<Date*> getBorrowedItemsTakenDates() const {return this->takenDates;}
    const vector<Date*> getBorrowedItemsReturnDates() const {return this->returnDates;}
    const vector<bool> getAreRealReturns() const {return this->areRealReturns;}

    //setters
    void setBorrowedItems(const vector<Book*>& borrowedItems) {this->borrowedItems = borrowedItems;}
    void setTakenDates(const vector<Date*>& takenDates) {this->takenDates = takenDates;}
    void setReturnDates(const vector<Date*>& returnDates) {this->returnDates = returnDates;}
    void setAreRealReturns(const vector<bool>& areRealReturns) {this->areRealReturns = areRealReturns;}

};


#endif //OOP_LIBRARY_PROJECT_READER_H
