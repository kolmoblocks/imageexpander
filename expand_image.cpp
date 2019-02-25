#include <iostream>
#include <cmath>
#include "lodepng.h"

bool isInt(float num) {
    return floor(num) == ceil(num);
}
struct color {
    unsigned char r,g,b;
};
// expand_image(bitmap_image, bitmap_image):
// recieves the old image and image diff as parameters to 
// construct the image of higher resolution.
void expand_image(const char *oldImgName, const char *diffImgName) {
    std::vector<unsigned char> oldImgVec, diffVec;
    unsigned diffW, diffH, oldImgH, oldImgW;

    unsigned error = lodepng::decode(diffVec, diffW, diffH, diffImgName, LCT_RGB, 8);
    if (error) {
          std::cout << lodepng_error_text(error) << std::endl;

       return;
   }
    
    unsigned cur = diffW+1;
    float curSqrt = sqrt(cur);

    while (true) {
        if (isInt(curSqrt)) {
            float smallSqrt = sqrt(cur - diffW);
            if (isInt(smallSqrt)) {
                break;
            }
        }
        ++cur;
        curSqrt = sqrt(cur);
    }

    error = lodepng::decode(oldImgVec, oldImgW, oldImgH, oldImgName, LCT_RGB, 8);
    if (error) {
          std::cout << lodepng_error_text(error) << std::endl;

       return;
   }

   
    unsigned loFactor = round(sqrt(cur-diffW));
    unsigned hiFactor = round(curSqrt);
    unsigned newW = oldImgW * hiFactor / loFactor;
    unsigned newH = oldImgH * hiFactor / loFactor;

    std::vector<struct color> newImgVec(newW * newH * 3);

    int diffX = 0;
    int diffY = 0;
    
    for (std::size_t y=0; y<newH; y+= hiFactor) {
      for (std::size_t x=0; x<newW; x+= hiFactor) {
         // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.
         // increment diffY, reset diffX to 0 each time we go to a new block
         ++diffY;
         diffX = 0;
         for (int innerX = x; innerX < x+hiFactor; ++innerX) {
            for (int innerY = y; innerY < y+hiFactor; ++innerY) {
               // only get and set pixel if the block is not included in the old block (for now it is the top left smaller square with sides of length "loFactor")
               if (!(innerX < x+loFactor) || !(innerY < y+loFactor)) {
                  // grab pixel from the diff
                   newImgVec[innerX + innerY*newW + 4].r = diffVec[3*(diffX + diffY*diffW)];
                    newImgVec[innerX + innerY*newW + 4].g = diffVec[3*(diffX + diffY*diffW)+1];
                    newImgVec[innerX + innerY*newW + 4].b = diffVec[3*(diffX + diffY*diffW)+2];
                  // increment diffX every time we increment through the inner block.
                  ++diffX;
               }
               else {
                   // grab pixel from the old image
                    newImgVec[innerX + innerY*newW].r = oldImgVec[ 3*(x * loFactor/hiFactor + (innerX - x) + oldImgW * (y * loFactor/hiFactor + (innerY - y)))];
                    newImgVec[innerX + innerY*newW].g = oldImgVec[ 3*(x * loFactor/hiFactor + (innerX - x) + oldImgW * (y * loFactor/hiFactor + (innerY - y))) + 1];
                    newImgVec[innerX + innerY*newW].b = oldImgVec[ 3*(x * loFactor/hiFactor + (innerX - x) + oldImgW * (y * loFactor/hiFactor + (innerY - y))) + 2];
               }
               
            }
         }
      }
   }
    std::cout << newW << ' ' << newH;
    std::vector<unsigned char> finalImage;
    for (auto it : newImgVec) {
        finalImage.push_back(it.r);
        finalImage.push_back(it.g);
        finalImage.push_back(it.b);
    }
    lodepng::encode("finale.png",finalImage,newW,newH,LCT_RGB,8);
}

int main(int argc, char **argv) {
    // argv[1] : smaller image, argv[2] : diff
    expand_image(argv[1],argv[2]);
}
