#pragma once
#include <openssl\rsa.h>

#define PUB_KEY "pubkey.pem"
#define PRI_KEY "prikey.pem"

/*************RSA�㷨***************/
// ������Կ��
void generateRSAKEY();
// ��ȡ��Կ
RSA* getPubKey();
// ��ȡ˽Կ
RSA* getPriKEy();
// ��Կ����
BYTE* RSAEncrypt(BYTE* data, RSA* rsa);
// ˽Կ����
BYTE* RSADecrypt(BYTE* data, RSA* rsa);

/*************MD5�㷨***************/
// ����
int MD5Encrypt(const void* data, size_t len, unsigned char* md5);

/*************AES�㷨***************/
// ����
int evpEncrypt(unsigned char* source_string, unsigned char* des_string, int length);
// ����
int evpDecrypt(unsigned char* source_string, unsigned char* des_string, int length);