#include "enhance.h"

char* getDiffFromFile(FILE *pFile){
    long lSize;
    char * buffer;
    size_t result = 0;

    pFile = fopen ( "diff.dat" , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    cout<<"lSize"<<lSize<<endl;
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
     fread (buffer,1,lSize,pFile);
    // for(int i = 0; i < lSize; i++) {
    //    fread(buffer+i, 1, 1, pFile); 
    // }
    //  for (int i = 0; i < 8 * lSize; i++){
    //      cout<<(int)buffer[i]<<endl;
    //  }
    fclose (pFile);
    free (buffer);
    return buffer;
}
void extractHeader(vector<unsigned char> diff, unsigned &highResWidth, unsigned &highResHeight) {
    vector<unsigned char> v;
    v.reserve(32);
    for (int i = 32; i < 64; i++){
        v.push_back(diff[i]);
    }
    highResWidth = (unsigned)binToInt(v);
    v.clear();
    for (int i = 64; i < 86; i++){
        v.push_back(diff[i]);       
    }
    highResHeight = (unsigned)binToInt(v);
    v.clear();
    for (int i = 86; i < 118; i++){
        v.push_back(diff[i]);
    }
    //colormode = assume RGB for now

}

vector<unsigned char> getBits(vector<unsigned char>buffer, int start, int len){
    vector<unsigned char> res;    
    res.reserve(len);
    for (int i = start; i < start + len; i++){
        res.push_back(buffer[i]); 
        cout<<(int)buffer[i];
    }

    
    cout<<endl;
    return res;
}



void populateDiffPixelVec(FILE *pFile, std::vector<unsigned char> &diffPixelVec, std::vector<unsigned char> &diffEncodedVec, unsigned deltaUnitSize, unsigned highResWidth, unsigned highResHeight) {
    // assuming is RANGE
    // assuming default config for a 16:9 image
    unsigned blockdataSize = highResHeight*highResWidth / (16*9);
    unsigned numBlocks = 16*9;
}



bool checkFileHeader(vector<unsigned char>diff){
    vector<unsigned char> v;
    string res = "";
    v = getBits(diff,0,8);
    res += (char)binToInt(v);
    cout<<"size"<<v.size()<<endl;
    v.clear();



    v = getBits(diff,8,8);
    res += (char)binToInt(v);
  
    v.clear();



    v = getBits(diff,16,8);
    res += (char)binToInt(v);
      for(auto it : v){
        cout<<it;
    }
    cout<<endl;
    v.clear();


    v = getBits(diff,24,8);
      for(auto it : v){
        cout<<it;
    }
    cout<<endl;
    res += (char)binToInt(v);


    cout<<res<<endl;
    return res == "DIFF";
}


void enhance(char *lowResFileName, char *diffFileName) {
    FILE *pDiff;
    pDiff = fopen(diffFileName, "rb");
    vector<unsigned char> deltas, diffPixelVec, lowResImage, diff;
    unsigned lowResWidth, lowResHeight, highResWidth, highResHeight,lowFactor, highFactor, 
    unitSize, deltaUnitSize, totalDeltaUnits, error;

    // rle decode diff, then put into diffFileVector

    char* buffer = getDiffFromFile(pDiff);
    
    diff.reserve(200);
    for (int i = 0; i < 200; i++){
        vector<unsigned char> bin;
        for (int j = 0; j < 8; j++){
            bin = intToUnsignedBin(buffer[i], 8);
           diff.insert(diff.end(), bin.begin(), bin.end());
        }
        
    }

    diff = decodeRLE(diff);


    if (!checkFileHeader(diff)){
        cerr<<"File is not in DIFF format"<<endl;
        return;
    }

    extractHeader(diff, highResWidth, highResHeight);

    cout<<highResWidth<<endl;
    cout<<highResHeight<<endl;


    error = lodepng::decode(lowResImage, lowResWidth, lowResHeight, lowResFileName, LCT_RGB, 8);
    lowFactor = lowResWidth / gcd(highResWidth, lowResWidth);
    highFactor = highResWidth / gcd(highResWidth, lowResWidth);
    
    unitSize = highFactor*highFactor;
    deltaUnitSize = unitSize - lowFactor*lowFactor;
    totalDeltaUnits = highResHeight*highResWidth / unitSize;
    // diff.reserve(3*totalDeltaUnits*deltaUnitSize);

    //iterate blocks
    // assuming aspect ratio is 16:9
    // assuming basic block configuration
    int numberOfDeltaPixelsPerBlock = unitSize - highResWidth*highResHeight*lowFactor*lowFactor/(16*9*highFactor*highFactor);
    // THIS SHOULD BE 3 SINCE WE ARE RESIZING 2:1
    


    
    
    
   
    
    
}



// void expand_image(const char *oldImgName, std::vector<unsigned char> diffVec, unsigned loFactor, unsigned hiFactor) {
//     std::vector<unsigned char> oldImgVec, diffVec;
//     unsigned oldImgH, oldImgW;

//     error = lodepng::decode(oldImgVec, oldImgW, oldImgH, oldImgName, LCT_RGB, 8);
//     if (error) {
//           std::cout << lodepng_error_text(error) << std::endl;

//        return;
//    }

   
//     unsigned newW = oldImgW * hiFactor / loFactor;
//     unsigned newH = oldImgH * hiFactor / loFactor;

//     std::vector<struct Color> newImgVec(newW * newH * 3);

//     int diffX = 0;
//     int diffY = 0;
    
//     for (std::size_t y=0; y<newH; y+= hiFactor) {
//       for (std::size_t x=0; x<newW; x+= hiFactor) {
//          // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.
//          // increment diffY, reset diffX to 0 each time we go to a new block
//          ++diffY;
//          diffX = 0;
//          for (int innerX = x; innerX < x+hiFactor; ++innerX) {
//             for (int innerY = y; innerY < y+hiFactor; ++innerY) {
//                // only get and set pixel if the block is not included in the old block (for now it is the top left smaller square with sides of length "loFactor")
//                if (!(innerX < x+loFactor) || !(innerY < y+loFactor)) {
//                   // grab pixel from the diff
//                    newImgVec[innerX + innerY*newW + 4].r = diffVec[3*(diffX + diffY*diffW)];
//                     newImgVec[innerX + innerY*newW + 4].g = diffVec[3*(diffX + diffY*diffW)+1];
//                     newImgVec[innerX + innerY*newW + 4].b = diffVec[3*(diffX + diffY*diffW)+2];
//                   // increment diffX every time we increment through the inner block.
//                   ++diffX;
//                }
//                else {
//                    // grab pixel from the old image
//                     newImgVec[innerX + innerY*newW].r = oldImgVec[ 3*(x * loFactor/hiFactor + (innerX - x) + oldImgW * (y * loFactor/hiFactor + (innerY - y)))];
//                     newImgVec[innerX + innerY*newW].g = oldImgVec[ 3*(x * loFactor/hiFactor + (innerX - x) + oldImgW * (y * loFactor/hiFactor + (innerY - y))) + 1];
//                     newImgVec[innerX + innerY*newW].b = oldImgVec[ 3*(x * loFactor/hiFactor + (innerX - x) + oldImgW * (y * loFactor/hiFactor + (innerY - y))) + 2];
//                }
               
//             }
//          }
//       }
//    }
//     std::vector<unsigned char> finalImage;
//     for (auto it : newImgVec) {
//         finalImage.push_back(it.r);
//         finalImage.push_back(it.g);
//         finalImage.push_back(it.b);
//     }
//     lodepng::encode("final.png",finalImage,newW,newH,LCT_RGB,8);
// }


