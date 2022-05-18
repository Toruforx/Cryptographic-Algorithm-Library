#include "test.h"
#include <iostream>
#include <cstring>


int main() {
    //******************Input*********************
    char Char_Key[9] = {0,};
    cout << "plz input Char_Key[8]" << endl;
    cin >> Char_Key;
    char Char_Mingwen[9] = { 0, };
    cout << "plz input Char_Mingwen[8]" << endl;
    cin >> Char_Mingwen;
    //******************Input********************
    DES_Key Input_Key(Char_Key);
    Des_Run(Char_Mingwen,Input_Key);
    system("pause");
    return 0;
}


DES_Key::DES_Key(char Char_Key[8]) {
    //将char类型的密码转换为Bit形式
    CharToBit(Bit_Key, Char_Key);
    //置换选择1
    Transform(Bit_Key, Bit_Key, PC1_Table, 56);
    bool *C0 = &Bit_Key[0];  //0~27
    bool *D0 = &Bit_Key[28]; //28~55
    for (int i = 0; i < 16; i++) {
        RotateL(C0, LOOP_Table[i]);
		for(int j = 0; j <28; j ++) {
			cout<<C0[j];
		}
		cout<<endl;
        RotateL(D0, LOOP_Table[i]);
        Transform(Round_Key[i], Bit_Key, PC2_Table, 48);
    }
}

void Xor(bool *InputA, const bool *InputB, int len) {
    for (int i = 0; i < len; i++) {
        InputA[i] = InputA[i] ^ InputB[i];
    }
}

void CharToBit(bool *Output, const char *Input) {
    for (int i = 0; i<64; i++) {
        Output[i] = (Input[i / 8] << (i % 8)) & 0x80;
    }
}

void Transform(bool *Output, bool *Input, const int *Table, int Tablelen) {
    bool temp[256];
    for (int i = 0; i < Tablelen; i++) {
        temp[i] = Input[Table[i] - 1];
    }
    memcpy(Output, temp, Tablelen);
}

void RotateL(bool *Input, int len) {
    bool temp[28];
    memcpy(temp, &Input[len], 28 - len);
    memcpy(&temp[28 - len], Input, len);
    memcpy(Input, temp, 28);
}

void S_func(bool *Output, bool *Input) {
    for (int i = 0; i < 8; i++) {
        int row = 2 * Input[6*i] + 1 * Input[6 * i+5];
        int col = 8 * Input[6*i+1] + 4 * Input[6 * i+2] + 2 * Input[6 * i+3] + 1 * Input[6 * i+4];
        int s_result = S_Box[i][row][col];
        for (int j = 0; j < 4; j++) {
            Output[4*i+j] = (s_result << j) & 0x8;
        }
    }
}

void F_func(bool *R, bool *Key) {
    bool Ex_R[48];
    //扩展置换
    Transform(Ex_R, R, E_Table, 48);
    Xor(Ex_R, Key, 48);
    S_func(R, Ex_R);
    Transform(R, R, P_Table, 32);
}

void Des_Run(char *Mingwen, DES_Key DES_Key) {
    bool Bit_Mingwen[64];
    CharToBit(Bit_Mingwen, Mingwen);
    //置换IP
    Transform(Bit_Mingwen, Bit_Mingwen, IP_Table, 64);
    bool *L0 = &Bit_Mingwen[0];
    bool *R0 = &Bit_Mingwen[32];
    //16轮迭代
    for (int i = 0; i < 16; i++) {
        bool temp[32];
        memcpy(temp, R0, 32);
        F_func(R0, DES_Key.Round_Key[i]);
        Xor(R0, L0, 32);
        memcpy(L0, temp, 32);
    }
    bool temp[32];
    memcpy(temp, R0, 32);
    memcpy(R0, L0, 32);
    memcpy(L0, temp, 32);
    Transform(Bit_Mingwen, Bit_Mingwen, IPR_Table, 64);
    ShowInHex(Bit_Mingwen);
}

void ShowInHex(const bool Input[64]) {
    unsigned int Hex[16] = { 0 };
    for (int i = 0; i<16; i++) {
        Hex[i] = 8 * Input[0 + i * 4] + 4 * Input[1 + i * 4] + 2 * Input[2 + i * 4] + 1 * Input[3 + i * 4];
    }
    for (int i = 0; i<16; i++) {
        cout << hex << Hex[i];
    }
    cout << endl;
}