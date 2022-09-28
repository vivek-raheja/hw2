#include <sstream>
#include <iomanip>
#include "movie.h"
#include "util.h"
#include <string>  
#include <stdio.h>

using namespace std;

Movie::Movie(const std::string category, const std::string name, double price, int qty, const std::string genre, const std::string rating):Product(category, name, price, qty)
{
	genre_ = genre;
	rating_ = rating;
}

Movie::~Movie()
{

}


string Movie::getGenre() const
{
    return genre_;
}

std::string Movie::getRating() const
{
    return rating_;
}

/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Movie::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}

void Movie::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << genre_ << "\n" << rating_ << endl;
}

std::set<std::string> Movie::keywords() const {
	std::set<std::string> words;
	std::set<std::string> keywords = parseStringToWords(name_);
	words.insert(keywords.begin(), keywords.end());
	keywords = parseStringToWords(genre_);
	words.insert(keywords.begin(), keywords.end());
	
	return words;
}
std::string Movie::displayString() const {
	std::string outStr = "Movie Name:";
	char tmp[100];
	
	outStr.append(name_);
	outStr.append(", Category:");
	outStr.append(category_);
	outStr.append(", Price:");
	sprintf(tmp, "%0.04f", price_);
	std::string str(tmp);
	outStr.append(str);
	outStr.append("\nGenre:");
	outStr.append(genre_);
	outStr.append(", Rating:");
	outStr.append(rating_);
	outStr.append("\n Quantity Available:");
	sprintf(tmp, "%d\n", qty_);
	std::string str2(tmp);
	outStr.append(str2);
	
	
	return outStr;
}