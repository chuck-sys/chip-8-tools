#include "Utilities.h"

bool isHexDigit(char c) {
    return ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'F'));
}
