#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 
#include <numeric> 
#include <string>
#include "blockIterator.h"

void insertMapBlock(vector<unsigned char> &diff, blockIterator &it, int minDelta, int maxDelta);