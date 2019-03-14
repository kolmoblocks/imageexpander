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
     int r, g, b;
} Color;

Color operator-(Color col1, Color col2) {
    Color col{col1.r-col2.r, col1.g-col2.g, col1.b-col2.b};
    return col;
}

Color max(Color col1, Color col2) {
    // sanity check for maximum delta
    Color col{max(col1.r, col2.r), max(col1.g, col2.g), max(col1.b, col2.b)};
    return col;
}

Color min(Color col1, Color col2) {
    // sanity check for minimum delta
    Color col{min(col1.r, col2.r), min(col1.g, col2.g), min(col1.b, col2.b)};
    return col;
}

class deltaUnit {
    int len;
    int ct=0;
    Color *colorDeltas;
    Color maxDeltas;
    Color minDeltas;
public:
    deltaUnit(int len): len(len) {
        colorDeltas = (Color*)malloc(len*sizeof(Color));
    }

    deltaUnit(int len, Color max, Color min): len{len}, maxDeltas{max}, minDeltas{min} {
        colorDeltas = (Color*)malloc(len*sizeof(Color));
    }
    void push_back(Color col) {
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
    void setMax(Color col) {
        maxDeltas = col;
    }
    void setMin(Color col) {
        minDeltas = col;
    }
    Color getMax() {
        return maxDeltas;
    }
    Color getMin() {
        return minDeltas;
    }
    int size() {
        return len;
    }
    Color &at(int pos) {
        if (pos < len && pos >= 0) {
            return colorDeltas[pos];
        }
        throw std::logic_error("requested delta Unit Color position is out of range.");
    }
};

#endif