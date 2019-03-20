#ifndef GENERATEDIFF_H
#define GENERATEDIFF_H
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <numeric>
#include <string>
#include "../bitUtil/bitUtil.h"
#include "../lodepng/lodepng.h"
#include "../rangeDiff/rangeDiff.h"
#include "../mapDiff/mapDiff.h"
#include "../blockIterator/blockIterator.h"
#include "../RLE/RLE.h"


using namespace std;

extern const char* lodepng_error_text(unsigned int);
extern unsigned lodepng::decode(std::vector<unsigned char>& out, unsigned& w, unsigned& h,
const std::string& filename,
        LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);

vector<unsigned char> intToBin(int c, int range) ;

void writeBit (int bit);

void padZeroes(vector <unsigned char> c);

void flushBits();

void insertBlockHeader(vector<unsigned char> &diff, int type, int rangeSize, int offset);

void insertDiffHeader(std::vector<unsigned char> &diff, unsigned int targetWidth, unsigned int targetHeight, string colormode);

vector<unsigned char> generateDiff (const char *lowRes, const char *highRes);


#endif