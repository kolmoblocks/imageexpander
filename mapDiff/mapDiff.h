#ifndef MAPDIFF_H
#define MAPDIFF_H
#include "../blockIterator/blockIterator.h"
#include "../lodepng/lodepng.h"
#include "../generateDiff/generateDiff.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set>
#include <numeric>
#include <string>

using namespace std;

void insertMapHeader(vector<unsigned char> &diff, int rangeSize, int offset);

void insertMapBlock(vector<unsigned char> &diff, blockIterator &it, int rangeSize, int offset);

#endif