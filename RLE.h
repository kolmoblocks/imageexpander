#ifndef RLE_H
#define RLE_H
#include <cmath>
#include <vector>
#include "bitUtil.h"
using namespace std;

vector <unsigned char> encodeRLE (vector<unsigned char> &bits);
vector <unsigned char> decodeRLE (vector<unsigned char> &bits);

#endif