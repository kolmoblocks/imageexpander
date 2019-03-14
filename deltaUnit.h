#ifndef DELTAUNIT_H
#define DELTAUNIT_H
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <numeric>

typedef struct {
    int x, y;
} posn;

bool operator!=(posn p1, posn p2);
typedef struct {
     int r, g, b;
} Color;

Color operator-(Color col1, Color col2);

int max(int a, int b);

int min(int a, int b);

Color max(Color col1, Color col2);

Color min(Color col1, Color col2);

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