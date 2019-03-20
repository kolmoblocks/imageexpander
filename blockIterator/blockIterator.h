#ifndef BLOCKITERATOR_H
#define BLOCKITERATOR_H
#include "../deltaUnit/deltaUnit.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <numeric>
using namespace std;

class blockIterator {
    std::vector<deltaUnit> &units;
public:
    posn pos, tl, br;
    unsigned int width, unitLength, innerUnitPos=0;
    
    blockIterator(std::vector<deltaUnit> &units, posn tl, posn br, unsigned int width);

    blockIterator &operator++();

    bool end();
    bool operator!=(blockIterator other);

    Color &operator*();

    void reset();
};

#endif