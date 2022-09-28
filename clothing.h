#ifndef CLOTHING_H
#define CLOTHING_H
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "product.h"

class Clothing: public Product {
public:
    Clothing(const std::string category, const std::string name, double price, int qty, const std::string size, const std::string brand);
    virtual ~Clothing();

	std::string getSize() const;
	std::string getBrand() const;
	
	virtual std::set<std::string> keywords() const;
	virtual std::string displayString() const;
	virtual void dump(std::ostream& os) const;
	virtual bool isMatch(std::vector<std::string>& searchTerms) const;
	
protected:
    std::string size_;
    std::string brand_;

};
#endif