#include "Date.h"
#include "Functionality.h"
#include <cstring>

ostream& operator << (ostream& os, const Date& date)
{

    for (int i = 0; i < 4 - Date::getNumLength(date.year); i++)
    {
        os << "0";
    }
    os << date.year;
    os << "-";

    for (int i = 0; i < 2 - Date::getNumLength(date.month); i++)
    {
        os << "0";
    }
    os << date.month;
    os << "-";

    for (int i = 0; i < 2 - Date::getNumLength(date.day); i++)
    {
        os << "0";
    }
    os << date.day;

    return os;
}

Date::Date(unsigned short day, unsigned short month, unsigned short year)
{
    this->day = day;
    this->month = month;
    this->year = year;
}

unsigned short Date::getNumLength(unsigned short num)
{
    unsigned short counter = 0;
    while (num)
    {
        counter++;
        num /= 10;
    }

    return counter;
}

Date& Date::operator=(const Date& other)
{
    if (this != &other)
    {
        this->day = other.day;
        this->month = other.month;
        this->year = other.year;
    }

    return *this;
}

bool operator<(const Date& d1, const Date& d2)
{
    if (d1.year < d2.year) return true;
    else if (d1.year == d2.year && d1.month < d2.month) return true;
    else if (d1.year == d2.year && d1.month == d2.month && d1.day < d2.day) return true;
    else return false;
}

bool operator==(const Date& d1, const Date& d2)
{
    if (d1.year == d2.year && d1.month == d2.month && d1.day == d2.day) return true;
    else return false;
}

Date* Date::getCurrentDate()
{
    time_t currentTime = time(0);
    tm* lt = localtime(&currentTime);
    return new Date(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
}

Date* Date::constructDateFromStr(char year[], char month[], char day[])
{
    return new Date(Functionality::toInt(day), Functionality::toInt(month), Functionality::toInt(year));
}
