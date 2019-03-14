#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

vector<unsigned char> lTrimZeroes(int c, int range) {
	vector<unsigned char> v;
    for (int i = range-1; i >= 0; i--){
            v.push_back((c & (1 << i)) != 0);
    }
	return v;
}


vector <unsigned char> encodeRLE (vector<unsigned char> &bits){
    vector <unsigned char> encoded;
    vector <unsigned char> binary;
    int len = bits.size();
    encoded.reserve(len);
    int i = 0, k;
    encoded.push_back(bits[0]);
    while (i <len) {
        k = 1;
        while (i + k < len && bits[i + k] == bits[i]){
            k++;
        }
        for (int l = 1; l <= floor(log(k)/log(2)); l++){
            encoded.push_back(0);
        }
        binary = lTrimZeroes(k, floor(log(k)/log(2)+1));
            encoded.insert(encoded.end(), binary.begin(), binary.end());
        // compressed.push_back();
        i+=k;
            
    }
    return encoded;
}

vector <unsigned char> decodeRLE (vector<unsigned char> &bits){
    unsigned char b = bits[0];
    for (int i = 1; i < bits.length();i++){
        int l = 0;
        while 
    }
}

int main(){
        vector <unsigned char> v;
    string answer = "10011101010000101000001011";
    string test = "11111110010000000000000000000011111111111";
    for (int i = 0; i < test.length(); i++){

        v.push_back(test[i]-'0');
    }
    for (auto it: v){
        cout<<(int)it;
    }
    cout<<endl;

    vector <unsigned char> encoded = encodeRLE(v); 
     for (auto it: encoded){
        cout<<(int)it;
    }
    
}

