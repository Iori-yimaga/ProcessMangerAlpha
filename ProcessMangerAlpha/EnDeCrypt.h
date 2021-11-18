#pragma once
#include <openssl\rsa.h>

#define PUB_KEY "pubkey.pem"
#define PRI_KEY "prikey.pem"

/*************RSA算法***************/
// 生成密钥对
void generateRSAKEY();
// 读取公钥
RSA* getPubKey();
// 读取私钥
RSA* getPriKEy();
// 公钥加密
BYTE* RSAEncrypt(BYTE* data, RSA* rsa);
// 私钥解密
BYTE* RSADecrypt(BYTE* data, RSA* rsa);

/*************MD5算法***************/
// 加密
int MD5Encrypt(const void* data, size_t len, unsigned char* md5);

/*************AES算法***************/
// 加密
int evpEncrypt(unsigned char* source_string, unsigned char* des_string, int length);
// 解密
int evpDecrypt(unsigned char* source_string, unsigned char* des_string, int length);