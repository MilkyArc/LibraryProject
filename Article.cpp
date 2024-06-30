#include "Article.h"
using std::endl;

std::ostream &operator<<(std::ostream& os, const Article& article)
{
    os << article.title << "|" << article.author << "|" << endl;
    os << article.keyWords.size() << endl;
    for (int i = 0; i < article.keyWords.size(); i++)
    {
        os << article.keyWords[i] << ((i != article.keyWords.size() - 1 ) ? "\n" : "");
    }

    return os;
}
