#ifndef OOP_LIBRARY_PROJECT_DATE_H
#define OOP_LIBRARY_PROJECT_DATE_H
#include <iostream>
#include <ctime>
using std::ostream;
//ISO 8601 format: 2022-06-15

class Date
{
private:
    unsigned short day;
    unsigned short month;
    unsigned short year;

    static unsigned short getNumLength(unsigned short num);

public:
    Date() {}
    Date(unsigned short day, unsigned short month, unsigned short year);
    Date& operator = (const Date&);
    friend ostream& operator << (ostream& os, const Date& date); // support for ISO8601
    friend bool operator < (const Date& d1, const Date& d2);
    friend bool operator == (const Date& d1, const Date& d2);

    static Date* getCurrentDate();
    static Date* constructDateFromStr(char year[], char month[], char day[]);

    //getters
    const unsigned short getDay() {return this->day;}
    const unsigned short getMonth() {return this->month;}
    const unsigned short getYear() {return this->year;}
};


#endif //OOP_LIBRARY_PROJECT_DATE_H
