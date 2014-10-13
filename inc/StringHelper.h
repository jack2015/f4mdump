
#ifndef _STRING_HELPER_H_
#define _STRING_HELPER_H_


#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

#include "SimpleDataTypes.h"

class CStringHelper
{
public:
    static int32_t atoi(const std::string &strNum);
    static bool is_number(const std::string &inStr);
    static void splitString(const std::string &inStr, std::vector<std::string> &outVect,
                            const char delim, const bool addEmpty);
    static std::string trim(const std::string &str, const std::string &whitespace = " \t\n\r");
    static bool startsWith(const std::string& haystack, const std::string& needle)
    {
        return needle.length() <= haystack.length() && std::equal(needle.begin(), needle.end(), haystack.begin());
    }
};

#endif //_STRING_HELPER_H_
