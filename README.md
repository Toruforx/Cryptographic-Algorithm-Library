# Cryptographic Algorithm Library

## Introduction

C Implementation of Encryption and Decryption of Multiple Cryptographic Algorithms

## List

|        | BUILD | TESTED | Encryption | Decryption |
| ------ | ----- | ------ | ---------- | ---------- |
| MD5    |   O   |    O   |     O      |     --     |
| SHA256 |   O   |    O   |     O      |     --     |
| DES    |   O   |    O   |     O      |     O      |
| AES    |   O   |    O   |     O      |     O      |
| RSA    |   O   |    O   |     O      |     O      |

## Environment

1. Operating system：Windows
2. Programming language：C, C++
3. Arbitrary precision arithmetic library：GMP
4. Code editor：vscode

## Program running

1. **compile and link**

   ```makefile
   gcc -c md5.c -o md5.o
   gcc md5.o -o md5
   
   gcc -c sha256.c -o sha256.o
   gcc sha256.o -o sha256
   
   gcc -c des.c -o des.o
   gcc des.o -o des
   
   gcc -c aes.c -o aes.o
   gcc aes.o -o aes
   
   g++ rsa.cpp -lgmp -o rsa 
   ```

2. **run the program**

   ```
   ./md5 <--encode> <text>
   
   ./sha256 <--encode> <text>
   
   ./des <--encode | --decode> <text> <key>
   
   ./aes <--encode | --decode> <text> <key>
   
   ./rsa <--run> <text>
   ```

## References

- 《密码编码与信息安全：C++实践》 王静文著
- 《现代密码学》(第4版) 杨波著
