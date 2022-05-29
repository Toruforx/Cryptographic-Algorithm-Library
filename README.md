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

1. Operating system：All
2. Programming language：C, C++
3. Arbitrary precision arithmetic library：GMP

## Program running

1. **compile and link**

   ```makefile
   make
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
