/** 
 * ioutils.h
 * @author: Yeray Alcala Paz
 * 
 * Archivo header e implementación con funciones
 *  útiles respecto al INPUT/OUTPUT.
 */
#ifndef IOUTILS_H
#define IOUTILS_H

#include <iostream>

using namespace std;

/**
 * Reads from standard input and clears
 *  buffer if fails.
 * 
 * @param str (in & out) char array where read data will be put.
 * @param SIZE (in) number of characteres to read.
 */
inline void read(char str[], const int SIZE) {
    cin.getline(str, SIZE+1);
    if(cin.fail()) {
        cin.clear();
        cin.ignore(256, '\n');
    }
}

#endif