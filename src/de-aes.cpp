#include <iostream>
#include <cstdio>
#include <cstring>
#include <map>
using namespace std;

const char str16[] = "0123456789abcdef";
map<char, int> m;

const unsigned int SR[16][16] = { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

const unsigned int Rcon[10] = { 0x01000000, 0x02000000,
    0x04000000, 0x08000000,
    0x10000000, 0x20000000,
    0x40000000, 0x80000000,
    0x1b000000, 0x36000000 };

const int colMR[4][4] = { 14, 11, 13, 9, 
    9, 14, 11, 13,
    13, 9, 14, 11,
    11, 13, 9, 14 };

unsigned int w[44] = {0};

void substituteR(unsigned int* state) {
    unsigned int tmp;
    for(int i = 0; i < 16; i ++) {
        tmp = state[i];
        state[i] = SR[tmp / 16][tmp % 16];
    }
    return;
}

void row_shiftR(unsigned int* state) {
    unsigned int tmp[16] = {0};
    for(int i = 0; i < 4; i ++) {
        for(int j = 0; j < 4; j ++) {
            tmp [4 * j + i] = state[(4 * j - 3 * i + 16) % 16];
        }
        for(int j = 0; j < 4; j ++) {
            state[4 * j + i] = tmp[4 * j + i];
        }
    }
}

void col_mixR(unsigned int* state) {
    unsigned int tmp, statep[16];
    for(int i = 0; i < 4; i ++) {
        for(int j = 0; j < 4; j ++) {
            tmp = 0;
            for(int k = 0; k < 4; k ++) {
                tmp ^= colMR[i][k] * state[k + 4 * j];
            }
            statep[i + 4 * j] = tmp;
        }
    }
    for(int i = 0; i < 16; i ++) {
        state[i] = statep[i];
    }
}

unsigned int T(unsigned int x, int t) {
    unsigned int tmp = 0;
    int r, c;
    x = ((x << 8) | (x >> 24)) & 0x0000ffff;
    for(int i = 0; i < 4; i ++) {
        c = x % 16;
        x >>= 4;
        r = x % 16;
        x >>= 4;
        tmp |= SR[r][c] << (8 * i);
    }
    tmp ^= Rcon[t];
    return tmp;
}

void generate_w(string str)
{
    unsigned int tmp, key[16];
    for(int i = 0; i < 16; i ++) {
        key[i] = (char)str[i];
        w[i / 4] |= key[i] << ((3 - i % 4) * 8);
    }
    for(int i = 4; i < 44; i ++) {
        if(i % 4 != 0) {
            w[i] = w[i - 4] ^ w[i - 1];
        }
        else {
            w[i] = w[i - 4] ^ T(w[i - 1], i / 4 - 1);
        }
        //cout<<w[i]<<endl;
    }
    return;
}

void add(unsigned int* state, int t) {
    unsigned int tmp = 0;
    for(int i = 0; i < 16; i ++) {
        tmp |= state[i] << ((3 - i % 4) * 8);
        if(i % 4 == 3) {
            tmp ^= w[4 * t + i / 4];
            for(int j = 0; j < 4; j ++) {
                state[i - j] = tmp % 64;
                tmp >>= 8;
            }
            tmp = 0;
        }
    }
}

string int_to_str(unsigned int* ct) {
    string ans = "";
    unsigned int res;
    for(int i = 0; i < 16; i ++) {
        res = ct[i];
        ans = ans + (char)res;
        //cout<<res<<endl;
    }
    return ans;
}

string decode(string strx, string strk) {
    unsigned int bit[16] = {0};
    for(int i = 0; i < 16; i ++) {
        bit[i] = m[strx[2 * i]];
        bit[i] = (bit[i] << 4) | m[strx[2 * i + 1]];
        //cout<<bit[i]<<endl;
    }
    generate_w(strk);
    add(bit,10);
    for(int t = 9; t > 0; t --) {
        row_shiftR(bit);
        substituteR(bit);
        add(bit,t);
        col_mixR(bit);
    }
    row_shiftR(bit);
    substituteR(bit);
    add(bit,0);
    return int_to_str(bit);
}

int main() {
    m['0'] = 0; m['1'] = 1; m['2'] = 2;m['3'] = 3;
    m['4'] = 4; m['5'] = 5; m['6'] = 6;m['7'] = 7;
    m['8'] = 8; m['9'] = 9; m['a'] = 10;m['b'] = 11;
    m['c'] = 12; m['d'] = 13; m['e'] = 14;m['f'] = 16;
    string strx, strk;//128bit - 16char
    /*
    cout << "Cipher Text: ";
    getline(cin, strx);
    cout << "Key Text: ";
    getline(cin, strk);
    */
    strx = "303104142c320b09010f03353f003d10";
    strk = "abcdefghijklmnop";
    cout << "result: " << decode(strx, strk) << endl;
}