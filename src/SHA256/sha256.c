#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 1000
unsigned int strlength, num;

const char str16[] = "0123456789abcdef";

unsigned int h[] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

const unsigned int K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

unsigned int rotateR(unsigned int a, unsigned int n) {
    return (a >> n) | (a << (32 - n));
}

unsigned int Ch(unsigned int X, unsigned int Y, unsigned int Z) {
    return (X & Y) ^ ((~X) & Z);
}

unsigned int Ma(unsigned int X, unsigned int Y, unsigned int Z) {
    return (X & Y) ^ (X & Z) ^ (Y & Z);
}

unsigned int sigma0(unsigned int X) {
    return rotateR(X, 2) ^ rotateR(X, 13) ^ rotateR(X, 22);
}

unsigned int sigma1(unsigned int X) {
    return rotateR(X, 6) ^ rotateR(X, 11) ^ rotateR(X, 25);
}

unsigned int round0(unsigned int X) {
    return rotateR(X, 7) ^ rotateR(X, 18) ^ (X >> 3);
}

unsigned int round1(unsigned int X) {
    return rotateR(X, 17) ^ rotateR(X, 19) ^ (X >> 10);
}

unsigned int* padding(char* str) {
    unsigned int slen = strlen(str);
    num = (slen * 8 + 64) / 512 + 1;
    strlength = num * 16;
    static unsigned int vec[MAX_LEN] = {0};
	for(unsigned int i = 0; i < slen; i ++) {
        //大端 256进制
        vec[i >> 2] |= (unsigned int)(str[i]) << ((3 - i % 4) * 8);
    }
    //补充1000..000
    vec[slen >> 2] |= 0x80 << ((3 - slen % 4) * 8);
    vec[strlength-1] = (slen << 3);
    return vec;
}

void update(unsigned int* ha, unsigned int res0, unsigned int res1) {
    ha[3] = (ha[3] + res0);
    for(int i = 7; i > 0; i --) {
        ha[i] = ha[i - 1];
    }
    ha[0] = res0 + res1;
}

void calculate(unsigned int *w) {
    unsigned int ha[8], res0, res1, ch, sig1, sig0, ma;
    for(int j = 0; j < 8; j ++)  {
        ha[j] = h[j];
    }
    for(int j = 0; j < 64; j ++) {
        ch = Ch(ha[4], ha[5], ha[6]);
        sig1 = sigma1(ha[4]);
        ma = Ma(ha[0], ha[1], ha[2]);
        sig0 = sigma0(ha[0]);
        res0 = (ha[7] + sig1 + ch + K[j] + w[j]);
        res1 = (sig0 + ma);
        update(ha, res0, res1);
    }
    for(int j = 0; j < 8; j ++) {
        h[j] += ha[j];
    }
}

char* uin_to_str(unsigned int* h) {
    static char ans[64];
    for(int j = 0; j < 8; j ++) {
        unsigned int x = h[j];
        for(int i = 0; i < 4; i ++) {
            unsigned int tmp = (x >> (8 * i)) % (1 << 8);
            ans[8 * j + 2 * i] = str16[(tmp / 16) % 16];
		    ans[8 * j + 2 * i + 1] = str16[tmp % 16];
        }
    }
    return ans;
}

char* encode(char* str) {
    unsigned int* vect = padding(str);
    unsigned int w[64];
    for(int i = 0; i < num; i ++) {
        for(int j = 0; j < 64; j ++) {
            if(j < 16)
                w[j] = vect[16 * i + j];
            else
                w[j] = round1(w[j - 2]) + w[j - 7] + round0(w[j - 15]) + w[j - 16];
        }
        calculate(w);
    }
    return uin_to_str(h);
}

int main() {
    char str[MAX_LEN];
	printf("Plain Text: ");
	gets(str);
	printf("Result: ");
	puts(encode(str));
}