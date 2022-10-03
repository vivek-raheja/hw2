#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "datastore.h"
#include "product.h"
#include "user.h"
#include <iostream>
#include "util.h"

using namespace std;

class MyDataStore: public DataStore {
public:
	virtual ~MyDataStore();

    /**
     * Adds a product to the data store
     */
    virtual void addProduct(Product* p) {
		allProducts.push_back(p);
		cout << "Added Product:" << p->getName() << "\n";
	}

    /**
     * Adds a user to the data store
     */
    virtual void addUser(User* u) {
		allUsers.push_back(u);
		cout << "Added User:" << u->getName() << "\n";
	}

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    virtual std::vector<Product*> search(std::vector<std::string>& terms, int type) {
		std::vector<Product*> data;
		
		std::set<std::string> setTerms(terms.begin(), terms.end());
		if(type == 1) {
			//OR Operation
			for(vector<Product*>::iterator it = allProducts.begin(); it != allProducts.end(); ++it) {
				std::set<std::string> keywords = (*it)->keywords();			
				//OR operation for all keywords individually
				std::set<std::string> outWords = setIntersection(keywords, setTerms);
				if(outWords.size() > 0) {
					data.push_back((*it));
				}
			}
		} else {
			//AND Operation
			for(vector<Product*>::iterator it = allProducts.begin(); it != allProducts.end(); ++it) {
				std::set<std::string> keywords = (*it)->keywords();
				bool allFound = true;
				
				set<std::string>::iterator itr;
				for (itr = setTerms.begin(); itr != setTerms.end(); itr++)
				{
					std::set<std::string> singleTerm;
					singleTerm.insert((*itr));
					std::set<std::string> outWords = setIntersection(keywords, singleTerm);
					if(outWords.size() == 0) {
						allFound  = false;
						break;
					}
				}
				if(allFound) {
					data.push_back((*it));
				}
			}
		}
		return data;
	}

    /**
     * Reproduce the database file from the current Products and User values
     */
    virtual void dump(std::ostream& ofile) {
		ofile << "\\\\New Database" << endl;
		ofile << "<products>" << endl;
		for(vector<Product*>::iterator it = allProducts.begin(); it != allProducts.end(); ++it) {
			Product *p = *it;
			p->dump(ofile);
		}
		ofile << "</products>" << endl;
		ofile << "<users>" << endl;
		for(vector<User*>::iterator it = allUsers.begin(); it != allUsers.end(); ++it) {
			User *p = *it;
			p->dump(ofile);
		}
		ofile << "</users>" << endl;
	}

	//Add variables to store the elements
	std::vector<Product*> allProducts;
	std::vector<User*> allUsers;
	
};
#endif