#include "lodepng.h"
#include <iostream>
using namespace std;

unsigned int gcd(unsigned int u, unsigned int v)
{
    // simple cases (termination)
    if (u == v)
        return u;

    if (u == 0)
        return v;

    if (v == 0)
        return u;

    // look for factors of 2
    if (~u & 1) // u is even
    {
        if (v & 1) // v is odd
            return gcd(u >> 1, v);
        else // both u and v are even
            return gcd(u >> 1, v >> 1) << 1;
    }

    if (~v & 1) // u is odd, v is even
        return gcd(u, v >> 1);

    // reduce larger argument
    if (u > v)
        return gcd((u - v) >> 1, v);

    return gcd((v - u) >> 1, u);
}

// generateDiff(string, string) 
// generates the diff, which is used by expand_image to expand the lower
// resolution image.
void generateDiff (const char *lowRes, const char *highRes){
   std::vector<unsigned char> lowResImage;
   std::vector<unsigned char> highResImage;
   unsigned lowResWidth, lowResHeight, highResWidth, highResHeight;

   unsigned error = lodepng::decode(lowResImage, lowResWidth, lowResHeight, lowRes);
   if (error) {
       std::cout << error;
       return;
   }

   error = lodepng::decode(highResImage, highResWidth, highResHeight, highRes);
   if (error) {
       std::cout << error;
       return;
   }

   unsigned denom = gcd(lowResWidth, highResWidth);
   // finding numerator and denominator of ratio (lowFactor, highFactor respectively)
   int lowFactor = lowResWidth / denom;
   int highFactor = highResWidth / denom;
   // finding diffWidth from the difference between smaller vs newer chunk sizes
   int diffWidth = highFactor * highFactor - lowFactor * lowFactor;
   int highResArea = highResWidth * highResHeight;

   // finding diffHeight from the number of chunks in a whole highRes image by finding the number of whole chunks (rounded up) for along the height and the width.
   int diffHeight = (highResWidth % highFactor == 0 ? highResWidth / highFactor : highResWidth / highFactor + 1 )
                  * (highResHeight % highFactor == 0 ? highResHeight / highFactor : highResHeight / highFactor + 1 );

   std::vector<unsigned char> diffVec;
   const unsigned int height = highResHeight;
   const unsigned int width  = highResWidth;
   // iterating through blocks, x and y indicate the top left positions of each block.
   for (std::size_t y=0; y<height; y+= highFactor) {
      for (std::size_t x=0; x<width; x+= highFactor) {
         // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.
         for (int innerX = x; innerX < x+highFactor; ++innerX) {
            for (int innerY = y; innerY < y+highFactor; ++innerY) {
               // only get and set pixel if the block is not included in the old block (for now it is the top left smaller square with sides of length "lowFactor")
               if (!(innerX < x+lowFactor) || !(innerY < y+lowFactor)) {
                  // set pixel of the diff at diffX , diffY with the color at the highResImage at innerX , innerY
                  diffVec.push_back(highResImage.at(innerX+innerY*highResWidth));
                  diffVec.push_back(highResImage.at(innerX+innerY*highResWidth + 1));
                  diffVec.push_back(highResImage.at(innerX+innerY*highResWidth + 2));
               }
            }
         }
      }
   }
   lodepng::encode("diff.png", diffVec, diffWidth, diffHeight);
}


int main(int argc, char *argv[]){
   generateDiff("1080p.png", "1440p.png");
}