#include "deltaUnit.h"

int max(int a, int b) {
    return (a>=b) ? a : b;
}

int min(int a, int b) {
    return (a<=b) ? a : b;
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

Color operator-(Color col1, Color col2) {
    Color col{col1.r-col2.r, col1.g-col2.g, col1.b-col2.b};
    return col;
}

bool operator!=(posn p1, posn p2) {
    return p1.x != p1.x || p1.y != p2.y;
}
