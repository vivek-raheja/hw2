#include <sstream>
#include <iomanip>
#include "book.h"
#include "util.h"
#include <string>  
#include <stdio.h>

using namespace std;

Book::Book(const std::string category, const std::string name, double price, int qty, const std::string isbn, const std::string author):Product(category, name, price, qty)
{
	isbn_ = isbn;
	author_ = author;
}

Book::~Book()
{

}


string Book::getISBN() const
{
    return isbn_;
}

std::string Book::getAuthor() const
{
    return author_;
}

bool Book::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}

void Book::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << isbn_ << "\n" << author_ << endl;
}

std::set<std::string> Book::keywords() const {
	std::set<std::string> words;
	std::set<std::string> keywords = parseStringToWords(name_);
	words.insert(keywords.begin(), keywords.end());
	words.insert(isbn_);
	keywords = parseStringToWords(author_);
	words.insert(keywords.begin(), keywords.end());
	
	return words;
}
std::string Book::displayString() const {
	std::string outStr = "Book Name:";
	char tmp[100];
	
	outStr.append(name_);
	outStr.append(", Category:");
	outStr.append(category_);
	outStr.append(", Price:");
	sprintf(tmp, "%0.04f", price_);
	std::string str(tmp);
	outStr.append(str);
	outStr.append("\nISBN:");
	outStr.append(isbn_);
	outStr.append(", Author:");
	outStr.append(author_);
	outStr.append("\n Quantity Available:");
	sprintf(tmp, "%d\n", qty_);
	std::string str2(tmp);
	outStr.append(str2);
	
	
	return outStr;
}