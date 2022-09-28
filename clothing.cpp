#include <sstream>
#include <iomanip>
#include "clothing.h"
#include "util.h"
#include <string>  
#include <stdio.h>

using namespace std;

Clothing::Clothing(const std::string category, const std::string name, double price, int qty, const std::string size, const std::string brand):Product(category, name, price, qty)
{
	size_ = size;
	brand_ = brand;
}

Clothing::~Clothing()
{

}


string Clothing::getSize() const
{
    return size_;
}

std::string Clothing::getBrand() const
{
    return brand_;
}

bool Clothing::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}

void Clothing::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << size_ << "\n" << brand_ << endl;
}

std::set<std::string> Clothing::keywords() const {
	std::set<std::string> words;
	std::set<std::string> keywords = parseStringToWords(name_);
	words.insert(keywords.begin(), keywords.end());
	keywords = parseStringToWords(brand_);
	words.insert(keywords.begin(), keywords.end());
	
	return words;
}
std::string Clothing::displayString() const {
	std::string outStr = "Clothing Name:";
	char tmp[100];
	
	outStr.append(name_);
	outStr.append(", Category:");
	outStr.append(category_);
	outStr.append(", Price:");
	sprintf(tmp, "%0.04f", price_);
	std::string str(tmp);
	outStr.append(str);
	outStr.append("\nSize:");
	outStr.append(size_);
	outStr.append(", Brand:");
	outStr.append(brand_);
	outStr.append("\n Quantity Available:");
	sprintf(tmp, "%d\n", qty_);
	std::string str2(tmp);
	outStr.append(str2);
	
	
	return outStr;
}