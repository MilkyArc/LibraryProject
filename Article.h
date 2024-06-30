#ifndef OOP_LIBRARY_PROJECT_ARTICLES_H
#define OOP_LIBRARY_PROJECT_ARTICLES_H

#include <iostream>
#include <vector>

using std::string;
using std::vector;

class Article
{
private:
    string title;
    string author;
    vector <string> keyWords;
public:
    Article()
    : title(""), author(""), keyWords() {}

    Article(string _title, string _author, vector <string> _keyWords)
    : title(_title), author(_author), keyWords(_keyWords) {}

    friend std::ostream& operator << (std::ostream& os, const Article& article);
};


#endif //OOP_LIBRARY_PROJECT_ARTICLES_H
