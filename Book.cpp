#include "Book.h"
#include "Functionality.h"

using std::cout;
using std::cin;
using std::endl;


Book::Book()
{
    author = "";
    title = "";
    publisher = "";
    genre = "";
    description = "";
    year = 0;
    rating = 0.0f;
    id = 0;
}

 Book::Book(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, unsigned short year, float rating)
{
    performSanityChecks(author, title, publisher, genre, desc, rating, year);

    this->author = author;
    this->title = title;
    this->publisher = publisher;
    this->genre = genre;
    this->description = desc;
    this->year = year;
    this->rating = rating;

    this->id = ++counter;
}

// overload for optional ISBN
Book::Book(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, unsigned short year, float rating, const string& ISBN)
{
    performSanityChecks(author, title, publisher, genre, desc, rating, year, ISBN);

    this->author = author;
    this->title = title;
    this->publisher = publisher;
    this->genre = genre;
    this->description = desc;
    this->year = year;
    this->rating = rating;
    this-> ISBN = ISBN;

    this->id = ++counter;
}

Book::Book(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, unsigned short year, float rating, const string& ISBN, unsigned short id)
{
    performSanityChecks(author, title, publisher, genre, desc, rating, year, ISBN);

    this->author = author;
    this->title = title;
    this->publisher = publisher;
    this->genre = genre;
    this->description = desc;
    this->year = year;
    this->rating = rating;
    this-> ISBN = ISBN;

    this->id = id;
}

void Book::performSanityChecks(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, float rating, unsigned short year)
{
    if (author.length() > 100) throw "Author is too long.";

    if(title.length() > 100) throw "Title is too long.";

    if(publisher.length() > 100) throw "Publisher is too long.";

    if(genre.length() > 45) throw "Genre is too long";

    if(desc.length() > 200) throw "Description is too long.";

    if (rating < 0.0f || rating > 10.0f) throw "invalid Book rating, expected [0.0 - 10.0]";

    if (year > Date::getCurrentDate()->getYear()) throw "Invalid data. Book can't be from the future.";

}

void Book::performSanityChecks(const string& author, const string& title, const string& publisher, const string& genre, const string& desc, float rating, unsigned short year, const string& ISBN)
{
    performSanityChecks(author, title, publisher, genre, desc, rating, year);
    if (ISBN.length() != 10 && ISBN.length() != 13 && ISBN.length() != 0) throw "ISBN invalid. Must be 10 or 13 chars.";
}

bool Book::operator==(const Book& book) const
{
    return (this->id == book.id);
}


std::ostream& operator<<(std::ostream& os, const Book& book)
{
    string type;
    if(typeid(book) == typeid(Book))
    {
        type = "book";
        os << "title: " << book.title << endl << "type: " << type << endl << "genre: " << book.genre << endl << "id: " << book.id << endl;
    }
    else
    {
        type =  "series";
        os << "title: "<< book.title << endl << "type:" << type << endl << "genre:" << book.genre << endl << "id:" << book.id << endl;
    }

    return os;
}


void Book::print(std::ofstream& os) const
{
    os << "Book"  << "|" << this->id <<  "|" << this->author << "|" << this->title << "|" << this->publisher << "|" << this->genre << "|" << this->description
       << "|" << this->year << "|" << this->rating << "|" << this->ISBN << endl << this->keyWords.size() << endl;

    for (int i = 0; i < this->keyWords.size(); i++)
    {
        os << this->keyWords[i] << endl;
    }
}

const string& Book::getPersonalNumber()
{
    return this->ISBN;
}

void Book::displayDetailed()
{
    cout << this->author << endl;
    cout << this->title << endl;
    cout << this->publisher << endl;
    cout << this->genre << endl;
    cout << this->description << endl;
    cout << this->year << endl;
    cout << this->rating << endl;
    cout << this->ISBN << endl;
    cout << "ID:" << this->id << endl;

    for (int i = 0; i < this->keyWords.size(); ++i)
    {
        cout << this->keyWords[i] << endl;
    }
}

void Book::addKeyWord(const string& str)
{
    this->keyWords.push_back(str);
    Functionality::writeItemsToFile();
}

Book* Book::getBookById(unsigned short id)
{
    for (int i = 0; i < allItemsCount; i++)
    {
        if (allItems[i]->getID() == id) return allItems[i];
    }

    return nullptr;
}

void Book::booksRemove(unsigned short id)
{
    if (Functionality::isLoggedIn && Functionality::isAdmin)
    {
        if (Reader::doesItemExist(id) && Reader::isItemAvailable(id))
        {
            vector<Book *>::iterator it1;
            for (int i = 0; i < allItemsCount; i++)
            {
                if (allItems[i]->id == id)
                {
                    it1 = allItems.begin() + i;
                    allItems.erase(it1);
                    Functionality::writeItemsToFile();
                    break;
                }
            }
        }
        else
        {
            cout << endl << "Item cannot be removed because it is currently taken or doesn't exist.";
            return;
        }
    }
    else
    {
        cout << endl << "Only admins can be remove items." << endl;
        return;
    }

    cout << endl << "Item with id: " << id << " removed successfully!" << endl;
}

void Book::booksAdd(const bool isSeries)
{
    if(Functionality::isLoggedIn && Functionality::isAdmin)
    {
        if (isSeries)
        {
            string title;
            string publisher;
            string genre;
            string desc;
            unsigned short year;
            unsigned short month;
            unsigned short edition;
            char rating[5];
            unsigned short articleCount;
            string ISSN;

            cout << endl << "Title: " << endl;
            Functionality::getLine(title);

            cout << "Publisher: " << endl;
            Functionality::getLine(publisher);

            cout << "Genre: " << endl;
            Functionality::getLine(genre);

            cout << "Short description: " << endl;
            Functionality::getLine(desc);

            cout << "Year of publishing: " << endl;
            cin >> year;

            cout << "Month of publishing: " << endl;
            cin >> month;

            cout << "Edition: " << endl;
            cin >> edition;

            cout << "Rating: " << endl;
            cin >> rating;

            cout << "How many articles would you like to add to this series?:" << endl;
            cin >> articleCount;

            vector<Article*> arts;
            for (int i = 0; i < articleCount ; i++)
            {
                string artTitle, artAuthor;
                unsigned short artKeyWordCount;
                string artKeyWord;

                cin.ignore();
                cout << "Author of article #" << i + 1 << endl;
                Functionality::getLine(artAuthor);

                cout << "Title of article #" << i + 1 << endl;
                Functionality::getLine(artTitle);

                cout << "How many keywords would you like to add for this article?:" << endl;
                cin >> artKeyWordCount;

                vector<string> artKeyWords;
                for (int j = 0; j < artKeyWordCount; j++)
                {
                    cout << "Enter keyword #" << j + 1 << " for article #" << i + 1 << ":" << endl;
                    cin >> artKeyWord;
                    artKeyWords.push_back(artKeyWord);
                }

                Article* art1 = new Article(artTitle, artAuthor, artKeyWords);
                arts.push_back(art1);
                delete art1;
            }

            string choice;
            do
            {
                cout << "Add ISSN? (y/n):" << endl;
                cin >> choice;
            }
            while (choice != "y" && choice != "n");

            if(choice == "y")
            {
                cout << "ISSN (8 chars):" << endl;
                cin >> ISSN;
                cin.ignore();

                PeriodicRelease::performSanityChecks(month, edition, ISSN);
                PeriodicRelease* s1 = new PeriodicRelease("", title, publisher, genre, desc, year, Functionality::toFloat(rating), month, edition, arts, ISSN);
                Functionality::addItem(s1);
                delete s1;
            }
            else
            {
                cin.ignore();
                PeriodicRelease::performSanityChecks(month, edition);
                PeriodicRelease* s2 = new PeriodicRelease("", title, publisher, genre, desc, year, Functionality::toFloat(rating), month, edition, arts);
                Functionality::addItem(s2);
                delete s2;
            }

            cout << "Series added successfully!" << endl;
        }




        else // book
        {
            string author;
            string title;
            string publisher;
            string genre;
            string desc;
            unsigned short year;
            unsigned short keyWordsCount = 0;
            vector<string> keyWords;
            char rating[5];
            string ISBN;

            cout << endl << "Author: " << endl;
            Functionality::getLine(author);

            cout << "Title: " << endl;
            Functionality::getLine(title);

            cout << "Publisher: " << endl;
            Functionality::getLine(publisher);

            cout << "Genre: " << endl;
            Functionality::getLine(genre);

            cout << "Short description: " << endl;
            Functionality::getLine(desc);

            cout << "Year of publishing: " << endl;
            cin >> year;

            cout << endl << "How many keywords would you like to add for this book:" << endl;
            cin >> keyWordsCount;

            for (int i = 0; i < keyWordsCount; i++)
            {
                string keyWord;
                cout << endl << "Enter keyword #" << i + 1 << ":" << endl;
                cin >> keyWord;
                keyWords.push_back(keyWord);
            }

            cout << endl << "Rating: " << endl;
            cin >> rating;

            string choice;
            do
            {
                cout << "Add ISBN? (y/n):" << endl;
                cin >> choice;
            }
            while (choice != "y" && choice != "n");

            if (choice == "y")
            {
                cout << "ISBN (10 or 13 chars):" << endl;
                cin >> ISBN;
                cin.ignore();
                Book::performSanityChecks(author, title, publisher, genre, desc, Functionality::toFloat(rating), year, ISBN);
                Book* b1 = new Book(author, title, publisher, genre, desc, year, Functionality::toFloat(rating), ISBN);
                for (int i = 0; i < keyWordsCount; i++)
                {
                    b1->addKeyWord(keyWords[i]);
                }

                Functionality::addItem(b1);
                delete b1;
            }
            else
            {
                cin.ignore();
                Book::performSanityChecks(author, title, publisher, genre, desc, Functionality::toFloat(rating), year);
                Book* b2 = new Book(author, title, publisher, genre, desc, year, Functionality::toFloat(rating));

                for (int i = 0; i < keyWordsCount; i++)
                {
                    b2->addKeyWord(keyWords[i]);
                }
                Functionality::addItem(b2);
                delete b2;
            }
            cout << "Book added successfully!" << endl;
        }
    }
    else
    {
        cout << "Only admins can add new books." << endl;
    }
}