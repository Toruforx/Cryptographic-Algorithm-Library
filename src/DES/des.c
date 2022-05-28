#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 1000

const char str16[] = "0123456789abcdef";
int m[250] = {0};
//置换IP表
const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};
//逆置换IP-1表
const int IPR[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};
// E位选择表(扩展置换表)
const int E[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};
// P换位表(单纯换位表)
const int P[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};
// PC1选位表(密钥生成置换表1)
const int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};
// PC2选位表(密钥生成置换表2)
const int PC2[48] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
//左移位数表
const int LOOP[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
// S盒
const int S[8][4][16] = {
    // S1
    14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
    0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
    4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
    15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,
    // S2
    15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
    3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
    0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
    13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,
    // S3
    10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
    13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
    13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
    1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
    // S4
    7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
    13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
    10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
    3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
    // S5
    2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
    14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
    4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
    11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
    // S6
    12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
    10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
    9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
    4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
    // S7
    4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
    13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
    1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
    6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
    // S8
    13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
    1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
    7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
    2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};

int L[17][32], R[17][32], C[17][28], D[17][28], K[17][48], FT[64];

void padding(char *str, int *rep)
{
    for (int i = 0; i < strlen(str) * 8; i++)
    {
        rep[i] = (str[i / 8] >> (7 - i % 8)) & 0x01;
    }
    return;
}

void ip_permutate(int *X)
{
    for (int i = 0; i < 32; i++)
    {
        L[0][i] = X[IP[i] - 1];
        R[0][i] = X[IP[i + 32] - 1];
    }
    return;
}

void key_generate(int *X)
{
    for (int i = 0; i < 28; i++)
    {
        C[0][i] = X[PC1[i] - 1];
        D[0][i] = X[PC1[i + 28] - 1];
    }
    for (int i = 1; i <= 16; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            C[i][j] = C[i - 1][(j + LOOP[i - 1]) % 28];
            D[i][j] = D[i - 1][(j + LOOP[i - 1]) % 28];
        }
    }
    for (int i = 1; i <= 16; i++)
    {
        for (int j = 0; j < 48; j++)
        {
            if (PC2[j] <= 28)
            {
                K[i][j] = C[i][PC2[j] - 1];
            }
            else
            {
                K[i][j] = D[i][PC2[j] - 29];
            }
        }
    }
    return;
}

void F(int *X, int *Y, int *f)
{
    int e[48], r, c, p[32], res;
    for (int i = 0; i < 48; i++)
    {
        e[i] = X[E[i] - 1];
        e[i] = (e[i] ^ Y[i]) & 0x01;
    }
    for (int i = 0; i < 8; i++)
    {
        r = (e[6 * i] << 1) | e[6 * i + 5];
        c = (e[6 * i + 1] << 3) | (e[6 * i + 2] << 2) | (e[6 * i + 3] << 1) | e[6 * i + 4];
        res = S[i][r][c];
        for (int j = 3; j >= 0; j--)
        {
            p[4 * i + j] = res % 2;
            res /= 2;
        }
    }
    for (int i = 0; i < 32; i++)
    {
        f[i] = p[P[i] - 1];
    }
    return;
}

void calculate_en(int t)
{
    int f[32];
    F(R[t - 1], K[t], f);
    for (int i = 0; i < 32; i++)
    {
        L[t][i] = R[t - 1][i];
        R[t][i] = L[t - 1][i] ^ f[i];
    }
    return;
}

void calculate_de(int t)
{
    int f[32];
    F(R[t - 1], K[17 - t], f);
    for (int i = 0; i < 32; i++)
    {
        L[t][i] = R[t - 1][i];
        R[t][i] = L[t - 1][i] ^ f[i];
    }
    return;
}

void ip_reverse(int *X, int *Y)
{
    for (int i = 0; i < 64; i++)
    {
        if (IPR[i] <= 32)
        {
            FT[i] = X[IPR[i] - 1];
        }
        else
        {
            FT[i] = Y[IPR[i] - 33];
        }
    }
}

char *int_to_str_en(int *ct)
{
    static char ans[17];
    ans[16] = '\0';
    int tmp;
    for (int i = 0; i < 16; i++)
    {
        tmp = 0;
        for (int j = 0; j < 4; j++)
        {
            tmp = 2 * tmp + ct[4 * i + j];
        }
        ans[i] = str16[tmp];
    }
    return ans;
}

char *int_to_str_de(int *ct)
{
    static char anst[9];
    anst[8] = '\0';
    int tmp;
    for (int i = 0; i < 8; i++)
    {
        tmp = 0;
        for (int j = 0; j < 8; j++)
        {
            tmp = 2 * tmp + ct[8 * i + j];
        }
        anst[i] = (char)tmp;
    }
    return anst;
}

char *des_encode(char *strx, char *strk)
{
    if (!(strlen(strx) == 8 && strlen(strk) == 8))
    {
        perror("invalid length");
        return NULL;
    }

    int bit[64] = {0}, key[64] = {0};
    padding(strx, bit);
    padding(strk, key);
    ip_permutate(bit);
    key_generate(key);
    for (int i = 1; i <= 16; i++)
    {
        calculate_en(i);
    }
    ip_reverse(R[16], L[16]);
    return int_to_str_en(FT);
}

char *des_decode(char *strx, char *strk)
{
    if (!(strlen(strx) == 16 && strlen(strk) == 8))
    {
        perror("invalid length");
        return NULL;
    }
    m['0'] = 0;
    m['1'] = 1;
    m['2'] = 2;
    m['3'] = 3;
    m['4'] = 4;
    m['5'] = 5;
    m['6'] = 6;
    m['7'] = 7;
    m['8'] = 8;
    m['9'] = 9;
    m['a'] = 10;
    m['b'] = 11;
    m['c'] = 12;
    m['d'] = 13;
    m['e'] = 14;
    m['f'] = 15;
    int bit[64] = {0}, key[64] = {0}, res;
    for (int i = 0; i < strlen(strx); i++)
    {
        res = m[strx[i]];
        for (int j = 3; j >= 0; j--)
        {
            bit[4 * i + j] = res % 2;
            res /= 2;
        }
    }
    padding(strk, key);
    ip_permutate(bit);
    key_generate(key);
    for (int i = 1; i <= 16; i++)
    {
        calculate_de(i);
    }
    ip_reverse(R[16], L[16]);
    return int_to_str_de(FT);
}

#if 1
int main(int argc, void *argv[])
{
    char *option, *text, *key, *out;
    // 64bit - 8char
    if (argc != 4)
    {
        printf("Usage: des <--encode | --decode> <text> <key>\n");
        return 0;
    }
    else
    {
        option = argv[1];
        text = argv[2];
        key = argv[3];
        if (strcmp(option, "--encode") == 0)
        {
            out = des_encode(text, key);
            puts(out);
        }
        else if (strcmp(option, "--decode") == 0)
        {
            out = des_decode(text, key);
            puts(out);
        }
        else
        {
            printf("Usage: des <--encode | --decode> <text> <key>\n");
            return 0;
        }
    }

    return 0;
}
#endif