
#ifndef IOUTILS_H
#define IOUTILS_H

#include <iostream>

using namespace std;

inline void read(char str[], const int SIZE) {
    cin.getline(str, SIZE+1);
    if(cin.fail()) {
        cin.clear();
        cin.ignore(256, '\n');
    }
}

#endif