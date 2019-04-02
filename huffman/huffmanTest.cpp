//
// Created by sam on 3/21/19.
//
#include "huffman.h"


int main(int argc, char **argv) {

//    ifstream f("diff.dat", ios::binary | ios::in);
//    FILE *encoded = fopen("diffhuff.dat", "w");
//    char c;

    std::vector<unsigned char> data = {0, 0, 0, 1, 0, 0, 0, 1,
                                       0, 1, 1, 0, 1, 1, 0, 1,
                                       1, 1, 0, 0, 1, 0, 1, 1,
                                       1, 1, 1, 0, 1, 0, 0, 1,
                                       1, 0, 1, 1, 0, 1, 1, 0};

    for (auto it : data) {
        std::cout << it;
    }
    std::cout << std::endl;
//    while (f.get(c)) {
//        // for (int i = 7; i >= 0;
//        for (int i = 0; i < 8; i++)  //if you want reverse bit order in bytes
//            data.push_back((c >> i) & 1);
//    }

    dNode *codex = encodeHuffman(data);

    std::vector<unsigned char> decoded;

    decodeHuffman(data, decoded, codex);

    for (auto it : decoded) {
        std::cout << it;
    }
    std::cout << std::endl;

    //writeBitsToFile(data, encoded);
}