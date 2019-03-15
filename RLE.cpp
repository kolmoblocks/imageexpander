#include <cmath>
#include <vector>
#include <iostream>
using namespace std;




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

int binToInt(vector<unsigned char> &bin){
    int exponent = bin.size() - 1;
    int x = 0;
    for (auto it: bin){
        x += (int)it * pow(2, exponent);
        exponent-=1;
    }
    return x;
}

vector <unsigned char> decodeRLE (vector<unsigned char> &bits){
    unsigned char b = bits[0];
    vector<unsigned char> decoded;
    vector<unsigned char> bin;
    int size = bits.size();

    for (int i = 1; i < size;){
        int l = 0;
        while (bits[i + l] == 0){
            l++;
        }
        if (size < i + 1) {
            cout<<"invalid encoding";
            return decoded;
        }
        for(int j = i+l; j< i + 2*l + 1; j++){
            bin.push_back(bits[j]);
        }
        int k = binToInt(bin);
        for (int j = 1; j <=k; j++){
            decoded.push_back(b);
        }
        i += 2*l + 1;
        b = 1 - b;
        bin.clear();
    }
    return decoded;
}

int main(){
    vector <unsigned char> v;
    string answer = "10011101010000101000001011";
    string test = "11111110010000000000000000000011111111111";
    vector<unsigned char> a {0,0,0,0,1,1,0,1,0,0,1,0,0,1,0,1,0};
    for (int i = 0; i < test.length(); i++){
        v.push_back(test[i]-'0');
    }

    vector <unsigned char> encoded = encodeRLE(v); 
     for (auto it: encoded){
        cout<<(int)it;
    }
    
    cout<<endl;

    vector <unsigned char> decoded = decodeRLE(a); 
     for (auto it: decoded){
        cout<<(int)it;
    }
    
}

