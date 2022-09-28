#ifndef UTIL_CPP
#define UTIL_CPP

#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "util.h"
#include <cstring>

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/
std::set<std::string> parseStringToWords(string rawWords)
{

	set<std::string> words;
	
	for(size_t i = 0; i < rawWords.size(); i++) {
		rawWords[i] = tolower(rawWords[i]);
    }
	
	std::string const delims = " .,:;!?'";

    size_t begin, pos = 0;
    while ((begin = rawWords.find_first_not_of(delims, pos)) != std::string::npos)
    {
        pos = rawWords.find_first_of(delims, begin + 1);
        string newWord = rawWords.substr(begin, pos - begin);
        if(newWord.size() >= 2) {
			words.insert(newWord);
		}
    }
	return words;
}

/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

#endif