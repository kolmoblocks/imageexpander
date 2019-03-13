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

class blockIterator {
    std::vector<deltaUnit> &units;
public:
    posn pos, tl, br;
    int width, unitLength, innerUnitPos=0;
    
    blockIterator(std::vector<deltaUnit> &units, posn tl, posn br, int width): 
        units{units}, pos{tl}, tl{tl}, br{br}, width{width}, unitLength{units[0].size()} {}

    blockIterator &operator++() {
        if (innerUnitPos < unitLength) {
            ++innerUnitPos;
        }
        else {
            innerUnitPos = 0;
            if (pos.x == br.x) {
                pos.x = tl.x;
                ++pos.y;
            }
            else if (pos.y > br.y || pos.x > br.x) {
                throw std::logic_error("block iterator position out of range");
            }
            else {
                ++pos.x;
            }
        }
        return *this;
    }

    bool end() {
        return innerUnitPos == unitLength-1 && pos.x == br.x && pos.y == br.y;
    }

    bool operator!=(blockIterator other) {
        return pos != other.pos;
    }

    color &operator*() {
        return units[pos.y*width + pos.x].at(innerUnitPos);
    }

    void reset() {
        pos = tl;
    }
};

#endif