#ifndef DELTAUNIT_H
#define DELTAUNIT_H
#include "rangeDiff/rangeDiff.h"
#include "generateDiff.h"
#include "deltaUnit.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <numeric> 

typedef struct {
    int x, y;
} posn;

bool operator!=(posn p1, posn p2) {
    return p1.x != p1.x || p1.y != p2.y;
}

typedef struct {
    short int r, g, b;
} color;

color &operator-(color col1, color col2) {
    color col{col1.r-col2.r, col1.g-col2.g, col1.b-col2.b};
    return col;
}

color &max(color col1, color col2) {
    // sanity check for maximum delta
    color col{max(col1.r, col2.r), max(col1.g, col2.g), max(col1.b, col2.b)};
    return col;
}

color &min(color col1, color col2) {
    // sanity check for minimum delta
    color col{min(col1.r, col2.r), min(col1.g, col2.g), min(col1.b, col2.b)};
    return col;
}

class deltaUnit {
    int len;
    int ct=0;
    color *colorDeltas;
    color maxDeltas;
    color minDeltas;
public:
    deltaUnit(int len): len(len) {
        colorDeltas = (color*)malloc(len*sizeof(color));
    }

    deltaUnit(int len, color max, color min): len{len}, maxDeltas{max}, minDeltas{min} {
        colorDeltas = (color*)malloc(len*sizeof(color));
    }
    void push_back(color col) {
        if (ct >= len) throw std::logic_error("pushing past specified deltaUnit length");
        colorDeltas[ct] = col;
        ++ct;
    }
    bool full() {
        return ct == len;
    }
    ~deltaUnit() {
        free(colorDeltas);
    }
    void setMax(color col) {
        maxDeltas = col;
    }
    void setMin(color col) {
        minDeltas = col;
    }
    color getMax() {
        return maxDeltas;
    }
    color getMin() {
        return minDeltas;
    }
    int size() {
        return len;
    }
    color &at(int pos) {
        if (pos < len && pos >= 0) {
            return colorDeltas[pos];
        }
        throw std::logic_error("requested delta Unit color position is out of range.");
    }
};

#endif