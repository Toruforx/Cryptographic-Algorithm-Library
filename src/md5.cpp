#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <cstdlib>
using namespace std;

#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

unsigned int strlength, num, FA, FB, FC, FD;

const char str16[] = "0123456789abcdef";
//4*16
const unsigned int T[] = {
    0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
    0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
    0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
    0x6b901122,0xfd987193,0xa679438e,0x49b40821,
    0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
    0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
    0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
    0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
    0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
    0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
    0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
    0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
    0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
    0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
    0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
    0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391 };

const unsigned int s[] = { 7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
                           5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
                           4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
                           6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};

unsigned int F(unsigned int X, unsigned int Y, unsigned int Z) {
    return (X & Y) | (~X & Z);
}

unsigned int G(unsigned int X, unsigned int Y, unsigned int Z) {
    return (X & Z) | (Y & ~Z);
}

unsigned int H(unsigned int X, unsigned int Y, unsigned int Z) {
    return X ^ Y ^ Z;
}

unsigned int I(unsigned int X, unsigned int Y, unsigned int Z) {
    return Y ^ (X | ~Z);
}

vector<unsigned int> padding(string str) {
    num = (str.length() * 8 + 64) / 512 + 1;
    strlength = num * 16;
    vector<unsigned int> vec(strlength);
    for(unsigned int i = 0; i < str.length(); i ++) {
        //小端 256进制
        vec[i >> 2] |= (unsigned int)(str[i]) << ((i % 4) * 8);
    }
    //补充1000..000
    vec[str.length() >> 2] |= 0x80 << ((str.length() % 4) * 8);
    unsigned int tmp = str.length() << 3;
    /*
    for(int i = 3; i >= 0; i --) {
        vec[vec.size() - 2] |= (tmp % (1 << 8)) << ((i % 4) * 8);
        tmp /= (1 << 8);
    }
    for(int i = 3; i >= 0; i --) {
        vec[vec.size() - 1] |= (tmp % (1 << 8)) << ((i % 4) * 8);
        tmp /= (1 << 8);
    }
    */
    vec[vec.size()-2] = (str.length() << 3);
    return vec;
}

string uin_to_str(unsigned int x) {
    string ans = "";
    for(int i = 0; i < 4; i ++) {
        string res = "";
        unsigned int tmp = (x >> (8 * i)) % (1 << 8);
        res = res + str16[(tmp / 16) % 16] + str16[tmp % 16];
        ans += res;
    }
    return ans;
}

unsigned int shift(unsigned int a, unsigned int n) {
    return (a << n) | (a >> (32 - n));
}

void update(unsigned int* md, int res, int t, int y) {
    for(int i = 4; i >= 2; i --) {
        md[i] = md[i - 1];
    }
    md[1] = md[1] + shift(md[0] + res + t + T[y], s[y]);
    md[0] = md[4];
}

void calculate(unsigned int* X) {
    unsigned int md[5], g, k;
    md[0] = A;
    md[1] = B;
    md[2] = C;
    md[3] = D;
    md[4] = 0;
    for(int i = 0; i < 16; i ++){
        g = F(md[1], md[2], md[3]);
        k = i;
        update(md, g, X[k], i);
    }
    for(int i = 16; i < 32; i ++){
        g = G(md[1], md[2], md[3]);
        k = (5 * i + 1) % 16;
        update(md, g, X[k], i);
    }
    for(int i = 32; i < 48; i ++){
        g = H(md[1], md[2], md[3]);
        k = (3 * i + 5) % 16;
        update(md, g, X[k], i);
    }
    for(int i = 48; i < 64; i ++){
        g = I(md[1], md[2], md[3]);
        k = (7 * i) % 16;
        update(md, g, X[k], i);
    }
    FA = A + md[0];
    FB = B + md[1];
    FC = C + md[2];
    FD = D + md[3];
}

string encode(string str) {
    vector<unsigned int> vec = padding(str);
    unsigned int tmp[16];
    for(unsigned int i = 0; i < num; i ++) {
        for(int j = 0; j < 16; j ++) {
            tmp[j] = vec[16 * i + j];
        }
        calculate(tmp);
    }
    /*
    cout<<FA<<endl;
    cout<<FB<<endl;
    cout<<FC<<endl;
    cout<<FD<<endl;
    */
    return uin_to_str(FA) + uin_to_str(FB) + uin_to_str(FC) + uin_to_str(FD);
}

int main() {
    string str;
    cout << "Plain Text: ";
    getline(cin,str);
    cout << "result: " << encode(str) << endl;
}