#ifndef ENHANCE_H
#define ENHANCE_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <numeric>
#include <string>
#include "bitUtil.h"
#include "lodepng.h"
#include "rangeDiff/rangeDiff.h"
#include "mapDiff.h"
#include "blockIterator.h"
#include "RLE.h"

void enhance(char *lowRes, char *diff);


#endif