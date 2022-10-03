#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);
bool addToCart(MyDataStore ds, string userName, Product* product);

std::map<std::string, vector<Product*> > currentCart;
std::map<std::string, vector<int> > currentCartQty;

bool checkUser(MyDataStore ds, string userName);
User* findUser(MyDataStore ds, string userName);
bool buyCart(MyDataStore ds, string userName);

bool viewCart(MyDataStore ds, string userName);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds = MyDataStore();

    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }
    

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ...                  " << endl;
    cout << "  OR term term ...                   " << endl;
    cout << "  ADD username search_hit_number     " << endl;
    cout << "  VIEWCART username                  " << endl;
    cout << "  BUYCART username                   " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                displayProducts(hits);
            }
            else if ( cmd == "OR" ) {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                displayProducts(hits);
            }
			else if ( cmd == "ADD" ) {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                string username = terms[0];
                int hit_index;
                stringstream obj;  
                obj << terms[1];
                obj >> hit_index;
                //terms[0] username
                //terms[1] index
                if((unsigned)hit_index <= hits.size() && hit_index > 0) {
					hit_index = hit_index - 1;
					if(addToCart(ds, username, hits[hit_index])) {
						cout << username << " ADDED PRODUCT " << (*hits[hit_index]).getName() << endl;
					} else {
						cout << "Check username" << endl;
					}
				} else {
					cout << "Select a proper product" << endl;
				}
            }
            else if ( cmd == "VIEWCART" ) {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                //terms[0] username
                string username = terms[0];
                viewCart(ds, username);
            }
            else if ( cmd == "BUYCART" ) {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                string username = terms[0];
                buyCart(ds, username);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
	    /* Add support for other commands here */
            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    if (hits.begin() == hits.end()) {
    	cout << "No results found!" << endl;
    	return;
    }
    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}

bool checkUser(MyDataStore ds, string userName) {
	for(vector<User*>::iterator it = ds.allUsers.begin(); it != ds.allUsers.end(); ++it) {
        if((*it)->getName() == userName) {
			return true;
		}
    }
    return false;
}

User* findUser(MyDataStore ds, string userName) {
	for(vector<User*>::iterator it = ds.allUsers.begin(); it != ds.allUsers.end(); ++it) {
        if((*it)->getName() == userName) {
			return *it;
		}
    }
    return NULL;
}

bool addToCart(MyDataStore ds, string userName, Product* product) {
	if(checkUser(ds, userName)) {
		//Check for product quantity
		int index = 0;
		int foundIndex = -1;
		for(vector<Product*>::iterator it = currentCart[userName].begin(); it != currentCart[userName].end(); ++it) {
			if((*it) == product) {
				//Product is already available
				foundIndex = index;
			}
			index++;
		}
		if(foundIndex == -1) {
			currentCart[userName].push_back(product);
			currentCartQty[userName].push_back(1);
		} else {
			currentCartQty[userName][foundIndex] = currentCartQty[userName][foundIndex] + 1;
		}
		return true; 
	}
	return false;
}

bool viewCart(MyDataStore ds, string userName) {
	if(checkUser(ds, userName)) {
		map<std::string, vector<Product*> >::iterator it = currentCart.find(userName);
		if(it != currentCart.end()) {
			vector<Product*> userCart = currentCart[userName];
			vector<int> userQty = currentCartQty[userName];
			cout << "YOUR CART" << endl;
			int itemCount = 1;
			for(vector<Product*>::iterator it = userCart.begin(); it != userCart.end(); ++it) {
				Product* product = *it;
				if(product == NULL)
					continue;
					
				cout << "Product " << itemCount << endl;
				cout << (*it)->displayString() << endl;
				cout << "YOU ARE PURCHASING " << userQty[itemCount-1] << " UNITS" << endl;
				cout << "**************" << endl;
				itemCount++;
			}
		} else {
			cout << "Cart is empty!" << endl;
		}
		return true;
	}
	return false;
}

bool buyCart(MyDataStore ds, string userName) {
	if(checkUser(ds, userName)) {
		User* user = findUser(ds, userName);
		
		map<std::string, vector<Product*> >::iterator it = currentCart.find(userName);
		if(it != currentCart.end()) {
			vector<Product*> userCart = currentCart[userName];
			vector<int> userQty = currentCartQty[userName];
			int counter = 0;
			
			for(vector<Product*>::iterator it = userCart.begin(); it != userCart.end(); ++it) {
				Product* product = *it;
				if(product == NULL)
					continue;
					
				if( product->getQty() > 0) {
					currentCart[userName][counter] = NULL;
					
					if(product->getPrice() < user->getBalance()) {
						user->deductAmount(product->getPrice());
						product->subtractQty(1);
						cout << "Product " << product->getName() << " purchase done" << endl;
					} else {
						cout << "Please recharge to buy " << product->getName() << "!" << endl;
					}
				} else {
					cout << "PRODUCT " << product->getName() << " NOT AVAILABLE!" << endl;
				}
				
				counter++;
			}
		} else {
			cout << "Cart is empty!" << endl;
		}
		return true;
	}
	return false;
}