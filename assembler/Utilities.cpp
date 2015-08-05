#include <cmath>

#include "Utilities.h"

bool isHexDigit(char c) {
    return ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f'));
}

int stringToHex(string seq) {
    int res = 0;
    for (int i=0; i<seq.size(); i++) {
        res += hexvals[seq.c_str()[i]]*pow(16, seq.size()-1-i);
    }
    return res;
}
