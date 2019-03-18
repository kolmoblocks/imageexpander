#include "enhance.h"

void extractInfo(FILE *pFile, unsigned &deltaUnitSize, unsigned &lowFactor, unsigned &highFactor) {

}



void enhance(char *lowResFileName, char *diffFileName) {
    FILE *pDiff;
    pFile = fopen(diffFileName);
    unsigned deltaUnitSize, lowFactor, highFactor;

    extractInfo(pDiff, deltaUnitSize, lowFactor, highFactor);
    
    unsigned lowResWidth, lowResHeight, highResWidth, highResHeight;
    std::vector<unsigned char> lowResImage;

    unsigned error = lodepng::decode(lowResImage, lowResWidth, lowResHeight, lowResFileName, LCT_RB, 8);
    
    
}

void expand_image(const char *oldImgName, std::vector<unsigned char> diffVec, unsigned loFactor, unsigned hiFactor) {
    std::vector<unsigned char> oldImgVec, diffVec;
    unsigned oldImgH, oldImgW;

    error = lodepng::decode(oldImgVec, oldImgW, oldImgH, oldImgName, LCT_RGB, 8);
    if (error) {
          std::cout << lodepng_error_text(error) << std::endl;

       return;
   }

   
    unsigned newW = oldImgW * hiFactor / loFactor;
    unsigned newH = oldImgH * hiFactor / loFactor;

    std::vector<struct Color> newImgVec(newW * newH * 3);

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
    std::vector<unsigned char> finalImage;
    for (auto it : newImgVec) {
        finalImage.push_back(it.r);
        finalImage.push_back(it.g);
        finalImage.push_back(it.b);
    }
    lodepng::encode("final.png",finalImage,newW,newH,LCT_RGB,8);
}



int main(int argc, char **argv) {
    
}