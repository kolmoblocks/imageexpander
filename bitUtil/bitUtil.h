#ifndef BITUTIL_H
#define BITUTIL_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "../RLE/RLE.h"

typedef struct {
    posn tl, br;
    int type;
} blockParams;

void writeBit (int bit, int &current_bit, FILE *f);

unsigned int gcd(unsigned int u, unsigned int v);

void writeBitsToFile (std::vector<unsigned char> &bitBuff, FILE *f);

std::vector<unsigned char> intToBin(int c, int range);

std::vector<unsigned char> intToUnsignedBin(int c, int range);

int binToInt(std::vector<unsigned char> &bin);

int binToSignedInt(std::vector<unsigned char> &bin);


#endif