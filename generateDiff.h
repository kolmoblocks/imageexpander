#ifndef GENERATEDIFF_H
#define GENERATEDIFF_H
#include "lodepng.h"
#include "rangeDiff/rangeDiff.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 
#include <numeric> 
#include <string>

using namespace std;

vector<unsigned char> lTrimZeroes(int c, int range) ;

void writeBit (int bit);

void padZeroes(vector <unsigned char> c);

void flushBits();

vector<unsigned char> generateDiff (const char *lowRes, const char *highRes, int height, int width, int x, int y);


#endif