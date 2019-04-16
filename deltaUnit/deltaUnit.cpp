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
    return p1.x != p2.x || p1.y != p2.y;
}


deltaUnit::deltaUnit(unsigned int len): len(len) {
    colorDeltas = (Color*)malloc(len*sizeof(Color));
}


deltaUnit::deltaUnit(unsigned int len, Color max, Color min): len{len}, maxDeltas{max}, minDeltas{min} {
    colorDeltas = (Color*)malloc(len*sizeof(Color));
}


void deltaUnit::push_back(Color col) {
    if (ct >= len) throw std::logic_error("pushing past specified deltaUnit length");
    maxDeltas = max(col, maxDeltas);
    minDeltas = min(col, minDeltas);
    colorDeltas[ct] = col;
    ++ct;
}


bool deltaUnit::full() {
    return ct == len;
}


deltaUnit::~deltaUnit() {
}


void deltaUnit::setMax(Color col) {
    maxDeltas = col;
}


void deltaUnit::setMin(Color col) {
    minDeltas = col;
}


Color deltaUnit::getMax() {
    return maxDeltas;
}


Color deltaUnit::getMin() {
    return minDeltas;
}


unsigned int deltaUnit::size() {
    return len;
}


Color &deltaUnit::at(int pos) {
    if (pos < len && pos >= 0) {
        return colorDeltas[pos];
    }
    std::string position = std::to_string(pos);
    std::string length = std::to_string(len);
    std::string msg = "delta is out of range: "+position+" of "+length;
    throw std::logic_error(msg);
}
