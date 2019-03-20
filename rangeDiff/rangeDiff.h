#ifndef RANGEDIFF_H
#define RANGEDIFF_H
#include "../blockIterator/blockIterator.h"
#include "../lodepng/lodepng.h"
#include "../generateDiff.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 
#include <numeric> 
#include <string>

using namespace std;

void insertRangeHeader(vector<unsigned char> &diff, int rangeSize, int offset);

void insertRangeBlock(vector<unsigned char> &diff, blockIterator &it, int rangeSize, int offset);

#endif