#include <cstdio>
#include <cmath>
#include "bitmap_image.hpp"
using namespace std;

bool isInt(float num) {
    return floor(num) == ceil(num);
}

// expand_image(bitmap_image, bitmap_image):
// recieves the old image and image diff as parameters to 
// construct the image of higher resolution.
void expand_image(bitmap_image oldImg, bitmap_image diff) {
    int diffW = diff.width();
    int diffH = diff.height();
    int cur = diffW;
    int curSqrt = sqrt(cur);

    while (true) {
        if (isInt(curSqrt)) {
            smallSqrt = cur - diffW;
            if (isInt(smallSqrt)) {
                break;
            }
        }
        ++cur;
        curSqrt = sqrt(cur);
    }

    int loFactor = round(sqrt(cur-diffW));
    int hiFactor = round(curSqrt);

    int newW = oldImg.width() * hiFactor / loFactor;
    int newH = oldImg.height() * hiFactor / loFactor;

    bitmap_image newImg(newW, newH);

    int diffX = 0;
    int diffY = 0;

    for (std::size_t y=0; y<height; y+= hiFactor) {
      for (std::size_t x=0; x<width; x+= hiFactor) {
         // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.
         // increment diffY, reset diffX to 0 each time we go to a new block
         ++diffY;
         diffX = 0;
         for (int innerX = x; innerX < x+hiFactor; ++innerX) {
            for (int innerY = y; innerY < y+hiFactor; ++innerY) {
               // only get and set pixel if the block is not included in the old block (for now it is the top left smaller square with sides of length "loFactor")
               if (!(innerX < x+loFactor) || !(innerY < y+loFactor)) {
                  // grab pixel from the diff
                  newImg.set_pixel(innerX, innerY, diff.get_pixel(diffX, diffY));
                  // increment diffX every time we increment through the inner block.
                  ++diffX;
               }
               else {
                   // grab pixel from the old image
                   newImg.set_pixel(innerX,innerY, oldImg.get_pixel(x * loFactor/hiFactor + (innerX - x), 
                                                                    y * loFactor/hiFactor + (innerY - y)));
               }
               
            }
         }
      }
   }

    newImg.save_image("expanded.bmp");
}

int main(int argc, char *argv[]){
    bitmap_image diff(argv[1]);
    bitmap_image old(argv[2]);
    expand_image(old, diff);
}