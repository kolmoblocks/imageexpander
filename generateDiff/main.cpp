#include "generateDiff.h"

int main(int argc, char *argv[]){
   // argv[1]: smaller file
    FILE *diffFile = fopen ("diff.dat", "wb");
    vector<unsigned char> diff;
    diff.reserve(239202 * 8);
    diff = generateDiff(argv[1], argv[2]);
    writeBitsToFile(diff, diffFile);
    fclose(diffFile);

}
