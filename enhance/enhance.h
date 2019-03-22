#ifndef ENHANCE_H
#define ENHANCE_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <numeric>
#include <string>
#include <cstdint>
#include "../bitUtil/bitUtil.h"
#include "../lodepng/lodepng.h"
#include "../rangeDiff/rangeDiff.h"
#include "../mapDiff/mapDiff.h"
#include "../blockIterator/blockIterator.h"
#include "../RLE/RLE.h"

void expandImage(const char *oldImgName, std::vector<unsigned char> &diff, unsigned lowFactor, unsigned highFactor);
void enhance(char *lowResFileName, char *diffFileName);


#endif