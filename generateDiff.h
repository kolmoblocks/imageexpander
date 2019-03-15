#ifndef GENERATEDIFF_H
#define GENERATEDIFF_H
#include "lodepng.h"
#include "rangeDiff/rangeDiff.h"
#include "mapDiff.h"
#include "blockIterator.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <numeric>
#include <string>
#include "bitUtil.h"

using namespace std;

extern const char* lodepng_error_text(unsigned int);
extern unsigned lodepng::decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
const std::string& filename,
        LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);

vector<unsigned char> intToBin(int c, int range) ;

void writeBit (int bit);

void padZeroes(vector <unsigned char> c);

void flushBits();

vector<unsigned char> generateDiff (const char *lowRes, const char *highRes, int height, int width, int x, int y);


#endif