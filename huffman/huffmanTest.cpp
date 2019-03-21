//
// Created by sam on 3/21/19.
//
#include "huffman.h"


int main(int argc, char **argv) {
    std::vector<unsigned char> binStr = {1,1,1,1,1,1,1,1,
                                         1,1,1,1,1,1,1,0,
                                         1,1,1,1,1,1,1,0,
                                         1,1,1,1,0,1,1,1,
                                         1,1,1,1,0,1,1,1,
                                         1,1,1,1,0,1,1,1,
                                         1,1,1,1,1,1,1,1,
                                         1,1,1,1,1,1,1,1};
    encodeHuffman(binStr);
}