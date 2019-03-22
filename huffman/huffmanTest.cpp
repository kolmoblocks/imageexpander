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
    dNode *codex = encodeHuffman(binStr);
    for (auto it : binStr) {
        std::cout << (int)it;
    }
    std::cout << std::endl;
}