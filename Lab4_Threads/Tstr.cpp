#pragma once

#include <string>
#include <cstring>

using namespace std;

char * str_to_cstr(string str) {
    
    char * cstr = new char[str.size() + 1];
    strcpy(cstr, str.c_str());
    
    return cstr;
}

char * int_to_cstr(int number) {
    
    string numStr = to_string(number);
    
    return str_to_cstr(numStr);
}
