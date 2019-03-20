#ifndef DELTAUNIT_H
#define DELTAUNIT_H
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <string>
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
    unsigned int len;
    int ct=0;
    Color *colorDeltas;
    Color maxDeltas;
    Color minDeltas;
public:
    deltaUnit(unsigned int len);
    deltaUnit(unsigned int len, Color max, Color min);
    void push_back(Color col);
    bool full();
    ~deltaUnit();
    void setMax(Color col);
    void setMin(Color col);
    Color getMax();
    Color getMin();
    unsigned int size();
    Color &at(int pos);
};

#endif