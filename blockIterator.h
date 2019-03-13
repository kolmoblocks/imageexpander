#ifndef BLOCKITERATOR_H
#define BLOCKITERATOR_H
#include "lodepng.h"
#include "rangeDiff/rangeDiff.h"
#include "generateDiff.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 
#include <numeric> 
#include <string>
class blockIterator {
    std::vector<deltaUnit> &units;
public:
    posn pos, tl, br;
    int width, unitLength, innerUnitPos=0;
    
    blockIterator(std::vector<deltaUnit> &units, posn tl, posn br, int width);

    blockIterator &operator++();

    bool end();
    bool operator!=(blockIterator other);

    color &operator*();

    void reset();
};
