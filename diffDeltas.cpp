#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 


using namespace std;

FILE *f;
int current_bit = 0;
unsigned char bit_buffer;

vector<char> LTrim_Zeroes(int c, int range) {
	vector<char> v;
    bool leadingZero = false;

    for (int i = range; i >= 0; i--){
        if (leadingZero){
            if ((c & (1 << i)) != 0) {
                v.push_back(1);
                leadingZero = true;
		    }  
        } else{
            v.push_back((c & (1 << i)) != 0);
        }
    }
	return v;
}

void WriteBit (int bit) {
    f = fopen ("uncompressed.dat", "w");
    if (bit)
        bit_buffer |= (1<<current_bit);

    current_bit++;
    if (current_bit == 8)
    {
        fwrite (&bit_buffer, 1, 1, f);
        current_bit = 0;
        bit_buffer = 0;
    }
}


void Flush_Bits(){
    while (current_bit < 8) {
        bit_buffer = (bit_buffer<<1);
          current_bit++;
    }
    fwrite (&bit_buffer, 1, 1, f);
    fclose (f); 

}


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
    f = fopen ("diff.dat", "w");
    
    std::vector<unsigned char> lowResImage;
    std::vector<unsigned char> highResImage;
    std::vector<unsigned char> diff;

    diff.reserve( 100000 );

    unsigned lowResWidth, lowResHeight, highResWidth, highResHeight;

    unsigned error = lodepng::decode(lowResImage, lowResWidth, lowResHeight, lowRes, LCT_RGB, 8);
    if (error) {
        std::cout << error;
        return;
    }

    error = lodepng::decode(highResImage, highResWidth, highResHeight, highRes, LCT_RGB, 8);
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

    const unsigned int height = highResHeight;
    const unsigned int width  = highResWidth;

    vector<int> block;
    unordered_set<int> deltaSet;

    // iterating through blocks, x and y indicate the top left positions of each block.
    for (std::size_t y=0; y<height; y+= highFactor) {
        for (std::size_t x=0; x<width; x+= highFactor) {
            // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.
           
            block.reserve(10);
            int maxDelta = 0;
            int r, g, b;
            for (int innerX = x; innerX < x+highFactor; ++innerX) {
                for (int innerY = y; innerY < y+highFactor; ++innerY) {
                // only get and set pixel if the block is not included in the old block (for now it is the top left smaller square with sides of length "lowFactor")
                    if (!(innerX < x+lowFactor) || !(innerY < y+lowFactor)) {
                        // set pixel of the diff at diffX , diffY with the color at the highResImage at innerX , innerY


                        r = (int)highResImage.at((innerX+innerY*highResWidth)*3);
                        g = (int)highResImage.at((innerX+innerY*highResWidth)*3+1);
                        b = (int)highResImage.at((innerX+innerY*highResWidth)*3+2);
                        
                        deltaSet.insert(r);
                        deltaSet.insert(g);
                        deltaSet.insert(b);

                        block.push_back(r);
                        block.push_back(g);
                        block.push_back(b);
                        
                        maxDelta = max(maxDelta,max(abs(r),abs(g),abs(b)));
                        


                    }
                }
            }

            float power = log(c)/log(2); // get the number of bits needed then + 1 for sign
	        int range = (int)floor(power) + 1;

            generateBlock(block, range,...some range start val);

            diff.insert( diff.end(), r.begin(), r.end() );
            diff.insert( diff.end(), g.begin(), g.end() );
            diff.insert( diff.end(), b.begin(), b.end() );


            block.clear();
            deltaSet.clear();
        }
    }

   std::cout << lodepng_error_text(error) << std::endl;
    fclose (f); 
}

void generateBlock(vector<unsigned char> &block, int range (+ 1 bit for unsigned), int refR, int refG, int refB){
    generate header
    - range size, range start

    calculate delta offset = max(0, lowest delta) - get the range start

    for (auto it: block){
         vector <char> r =  LTrim_Zeroes(){
             LTRIM then add 1 bit for sign
         }
    }

}



int main(int argc, char *argv[]){
   // argv[1]: smaller file
   generateDiff(argv[1], argv[2]);
}
