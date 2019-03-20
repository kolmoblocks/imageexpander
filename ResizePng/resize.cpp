#include <iostream>
#include <sstream>
#include "lodepng.h"

int main(int argc, char **argv) {
    char *imgFile = argv[1];
    int r1, r2;
    
    std::stringstream ratio2(argv[2]);
    std::stringstream ratio1(argv[3]);
    ratio1 >> r1;
    ratio2 >> r2;

    std::vector<unsigned char> imgVec;
    std::vector<unsigned char> newImgVec;

    int height, width;
    unsigned ht, wid;
    unsigned err = lodepng::decode(imgVec,  wid, ht, imgFile, LCT_RGB, 8);
    std::cout << err << std::endl;

    height = ht;
    width = wid;
    for (int i=0; i<height * width; i+=r2*width) {
        for (int j=0; j<r1*width; j+=width) {
            for (int k=0; k<width; k+=r2){
                for (int l=0; l<r1; ++l) {
                    newImgVec.push_back(imgVec[(i+j+k+l)*3]);
                    newImgVec.push_back(imgVec[(i+j+k+l)*3 + 1]);
                    newImgVec.push_back(imgVec[(i+j+k+l)*3 + 2]);
                }
            }
        }
    }
    std::cout << newImgVec.size() << std::endl;
    unsigned ur1, ur2;
    ur1 = r1;
    ur2 = r2;
    std::cout << ht*ur1/ur2 << " " << width*r1/r2 << std::endl;
    err = lodepng::encode("resized.png", newImgVec,  wid*ur1/ur2, ht*ur1/ur2, LCT_RGB, 8);
    std::cout << lodepng_error_text(err) << std::endl;
}