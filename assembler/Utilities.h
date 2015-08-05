#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <string>
#include <map>

using namespace std;

bool isHexDigit(char);
int stringToHex(string);

static map<char, int> hexvals = {
    {'0', 0},
    {'1', 1},
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'a', 10},
    {'b', 11},
    {'c', 12},
    {'d', 13},
    {'e', 14},
    {'f', 15},
};



#endif
