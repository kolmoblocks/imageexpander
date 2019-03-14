#ifndef MAPDIFF_H
#define MAPDIFF_H
#include <iostream>
#include <fstream>
#include <cmath>
#include <numeric>
#include <string>
#include "blockIterator.h"
using namespace std;
void insertMapBlock(vector<unsigned char> &diff, blockIterator &it, int minDelta, int maxDelta);

#endif