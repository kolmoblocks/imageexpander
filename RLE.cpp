#include <cmath>
#include <vector>
using namespace std;


vector <unsigned char> RLE (vector<unsigned char> &bits){
    vector <unsigned char> compressed;
    int len = bits.size();
    compressed.reserve(len);
    int i = 0, k;
    while (i <len) {
        k = 1;
        while (i + k < len && bits[i + k] == bits[i]){
            k++;
        }
        for (int l = 0; i < floor(log(k)/log(2)); l++){
            compressed.push_back(0);
        }
        compressed.push_back(binary encoding of k);
        i+=k;
            
    }

    return compressed;
}

