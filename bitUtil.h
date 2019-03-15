#ifndef BITUTIL_H
#define BITUTIL_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>


void writeBit (int bit, int &current_bit, FILE *f);

void writeBitsToFile (std::vector<unsigned char> &bitBuff, FILE *f);

std::vector<unsigned char> intToBin(int c, int range);
int binToInt(std::vector<unsigned char> &bin);

#endif