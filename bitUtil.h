#ifndef BITUTIL_H
#define BITUTIL_H

#include <iostream>
#include <fstream>
#include <cmath>

void writeBit (int bit, int &current_bit, FILE *f);

void writeBitsToFile (std::vector<unsigned char> &bitBuff, FILE *f);

vector<unsigned char> intToBin(int c, int range);

#endif