#include "RLE.h"
int main(){
    vector <unsigned char> v;
    string a = "0000101010100011101010101010101010100110";
    string b = "11111110010000000000000000000011111111111";
    // vector<unsigned char> a {0,0,0,0,1,1,0,1,0,0,1,0,0,1,0,1,0};
    for (int i = 0; i < a.length(); i++){
        v.push_back(a[i]-'0');
    }
     for (auto it: v){
        cout<<(int)it;
    }
    
    cout<<endl;

    vector <unsigned char> encoded = encodeRLE(v); 
     for (auto it: encoded){
        cout<<(int)it;
    }

    cout<<endl;

    vector <unsigned char> decoded = decodeRLE(encoded); 
     for (auto it: decoded){
        cout<<(int)it;
    }
    
    cout<<endl; 
    
}
