#pragma once

#include <string>

class Utils {
public:
    static void trimString( std::string & str ) {
        const char * whiteSpace = " \t\n\r";
        size_t location;
        location = str.find_first_not_of(whiteSpace);
        str.erase(0,location);
        location = str.find_last_not_of(whiteSpace);
        str.erase(location + 1);
    }
};