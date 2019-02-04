#include "bitmap_image.hpp"
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
void generateDiff (string lowRes, string highRes){
   bitmap_image lowResImage(lowRes);
   bitmap_image highResImage(highRes);
   
   unsigned int denom = gcd(lowResImage.width(), highResImage.width());
   // finding numerator and denominator of ratio (lowFactor, highFactor respectively)
   int lowFactor = lowResImage.width() / denom;
   int highFactor = highResImage.width() / denom;
   // finding diffWidth from the difference between smaller vs newer chunk sizes
   int diffWidth = highFactor * highFactor - lowFactor * lowFactor;
   int highResArea = highResImage.width() * highResImage.height();

   // finding diffHeight from the number of chunks in a whole highRes image by finding the number of whole chunks (rounded up) for along the height and the width.
   int diffHeight = (highResImage.width() % highFactor == 0 ? highResImage.width() / highFactor : highResImage.width() / highFactor + 1 )
                  * (highResImage.height() % highFactor == 0 ? highResImage.height() / highFactor : highResImage.height() / highFactor + 1 );

   bitmap_image diff(diffWidth, diffHeight);

   if (!highResImage){
      printf("Error - Failed to open file\n");
   }

   const unsigned int height = highResImage.height();
   const unsigned int width  = highResImage.width();
   // iterating through blocks, x and y indicate the top left positions of each block.
   int diffX = 0;
   int diffY = 0;
   rgb_t colour;
   for (std::size_t y=0; y<height; y+= highFactor) {
      for (std::size_t x=0; x<width; x+= highFactor) {
         // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.
         // increment diffY, reset diffX to 0 each time we go to a new block
         ++diffY;
         diffX = 0;
         for (int innerX = x; innerX < x+highFactor; ++innerX) {
            for (int innerY = y; innerY < y+highFactor; ++innerY) {
               // only get and set pixel if the block is not included in the old block (for now it is the top left smaller square with sides of length "lowFactor")
               if (!(innerX < x+lowFactor) || !(innerY < y+lowFactor)) {
                  // set pixel of the diff at diffX , diffY with the color at the highResImage at innerX , innerY
                  diff.set_pixel(diffX, diffY, highResImage.get_pixel(innerX, innerY));
                  //cout << diffX << " " << diffY << " " << highResImage.get_pixel(innerX, innerY).blue << endl;
                  // increment diffX every time we increment through the inner block.
                  ++diffX;
               }
            }
         }
      }
   }
   diff.save_image("diff.bmp");
}


int main(int argc, char *argv[]){
   generateDiff("360p.bmp", "720p.bmp");
}