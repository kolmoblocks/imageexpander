#include "emscripten.h"
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
void expand_image_to_buffer(bitmap_image oldImg, bitmap_image diff, char* pOutBuffer, int &BufferLength ) {
    int diffW = diff.width();
    int diffH = diff.height();
    int cur = diffW+1;
    int curSqrt = sqrt(cur);

    while (true) {
        if (isInt(curSqrt)) {
            float smallSqrt = cur - diffW;
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

    newImg.save_image_to_buffer(pOutBuffer, BufferLength);
}

/*int main(int argc, char *argv[]){
    bitmap_image diff(argv[2]);
    bitmap_image old(argv[1]);
    expand_image(old, diff);
}*/

extern "C" {
    int* set_arg(uint8_t arg_index, int size);
    int* get_result();
    int get_result_size();
    int exec();
}

char* g_Old = NULL;
int g_Old_size = 0;

char* g_Diff = NULL;
int g_Diff_size = 0;

char* g_Result = NULL;
int g_Result_size = 0;


int* set_arg(uint8_t arg_index, int size)
{
    if ( 0 == arg_index ) // old
    {
        g_Old = new char[size];
        
        g_Old_size = size;

        return (int*)g_Old;
    }

    if ( 1 == arg_index ) // diff
    {
        g_Diff = new char[size];
        
        g_Diff_size = size;

        return (int*)g_Diff;
    }

    return NULL;
}


int* get_result()
{
    return (int*)g_Result;
}


int get_result_size()
{
    return g_Result_size;
}


int exec()
{
    if ( g_Diff && g_Old )
    {
        bitmap_image old;
        if ( !old.LoadFromBuffer(g_Old, g_Old_size) )
            return false;

        bitmap_image diff;
        if ( !diff.LoadFromBuffer(g_Diff, g_Diff_size) )
            return false;

        expand_image_to_buffer(old, diff, g_Result, g_Result_size );

        return true;
    }

    return false;
}