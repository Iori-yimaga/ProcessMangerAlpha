#include "pch.h"
#include <openssl\pem.h>
#include <openssl\rand.h>
#include <openssl\md5.h>
#include <openssl\bio.h>
#include <openssl\evp.h>
#include <openssl\applink.c>
#pragma comment(lib, "libcrypto.lib")
#include "EnDeCrypt.h"

// 生成密钥对
void generateRSAKEY() {
	RSA* keypair = RSA_generate_key(
		1024,    // 密钥长度
		RSA_F4,  // 公钥指数
		NULL,
		NULL
	);
	// 从生成的密钥对中读取私钥到内存对象
	BIO* pri = BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
	// 获取密钥长度并且申请空间进行保存
	size_t priLen = BIO_pending(pri);
	char* priKey = (char*)calloc(priLen + 1, sizeof(char));
	BIO_read(pri, priKey, priLen);
	// 将生成的私钥写入到指定的文件中
	FILE* priFile = NULL;
	if (fopen_s(&priFile, PRI_KEY, "w") == NULL) {
		if (priKey && priFile) {
			fputs(priKey, priFile);
			fclose(priFile);
		}
	}
	// 将生成的密钥对中读取公钥到内存对象
	BIO* pub = BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPublicKey(pub, keypair);
	size_t pubLen = BIO_pending(pub);
	char* pubKey = (char*)calloc(pubLen + 1, sizeof(char));
	BIO_read(pub, pubKey, pubLen);
	FILE* pubFile = NULL;
	if (fopen_s(&pubFile, PUB_KEY, "w") == NULL) {
		if (pubKey && pubFile) {
			fputs(pubKey, pubFile);
			fclose(pubFile);
		}
	}
	// 释放资源
	RSA_free(keypair);
	BIO_free_all(pub);
	BIO_free_all(pri);
	free(priKey);
	free(pubKey);
}

// 获取公钥
RSA* getPubKey() {
	// 打开公钥文件
	FILE* pubFile = NULL;
	if (fopen_s(&pubFile, PUB_KEY, "r") == NULL) {
		// 从指定文件中读取公钥
		RSA* rsa = PEM_read_RSAPublicKey(pubFile, NULL, NULL, NULL);
		if (pubFile) fclose(pubFile);
		return rsa;
	}
	return nullptr;
}

// 获取私钥
RSA* getPriKEy() {
	// 打开私钥文件
	FILE* priFile = NULL;
	if (fopen_s(&priFile, PRI_KEY, "r") == NULL) {
		// 从指定文件中读取公钥
		RSA* rsa = PEM_read_RSAPrivateKey(priFile, NULL, NULL, NULL);
		if (priFile) fclose(priFile);
		return rsa;
	}
	return nullptr;
}

// 加密
BYTE* RSAEncrypt(BYTE* data, RSA* rsa) {
	int rsaLen = RSA_size(rsa);
	BYTE* encrypt = (BYTE*)malloc(rsaLen);
	RSA_public_encrypt(117, data, encrypt, rsa, RSA_PKCS1_PADDING);

	return encrypt;
}

// 解密
BYTE* RSADecrypt(BYTE* data, RSA* rsa) {
	int rsaLen = RSA_size(rsa);
	BYTE* decrypt = (BYTE*)malloc(rsaLen);
	RSA_private_decrypt(rsaLen, data, decrypt, rsa, RSA_PKCS1_PADDING);

	return decrypt;
}

// MD5加密
int MD5Encrypt(const void* data, size_t len, unsigned char* md5) {
	// 初始化保存 md5 信息的结构体
	MD5_CTX ctx = { 0 };
	MD5_Init(&ctx);
	// 将需要计算的数据传入到对应的结构中
	MD5_Update(&ctx, data, len);
	// 从结构中获取计算后的结果
	MD5_Final(md5, &ctx);
	return 0;
}

// 返回值是加密后的密文长度，传入明文，明文长度和保存密文的缓冲区
// AES 加密
int evpEncrypt(unsigned char* source_string, unsigned char* des_string, int length){
	// 创建一个通用加解密的对象，设置填充方式
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx, 1);

	// once_length 是一次加密的长度, out_length 加密后密文的长度 
	int once_length = 0, out_length = 0;

	// 设置加密时使用的算法+分组模式和 key 函数的倒数第二个参数可以
	// 用于指定初始化向量，最后一个参数是 1 表示加密，否则解密
	unsigned char key[16] = "15pb";
	EVP_CipherInit_ex(ctx, EVP_des_ede3_ecb(), NULL, key, nullptr, 1);

	// 使用 EVP_CipherUpdate + EVP_CipherFinal 完成整个加密
	EVP_CipherUpdate(ctx, des_string, &once_length, source_string, length);
	out_length += once_length;
	EVP_CipherFinal(ctx, des_string + once_length, &once_length);
	out_length += once_length;

	// 清理对象并返回长度
	EVP_CIPHER_CTX_free(ctx);
	return out_length;
}

// AES解密
int evpDecrypt(unsigned char* source_string, unsigned char* des_string, int length){
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx, 1);

	int once_length = 0, out_length = 0;

	// 解密的代码和加密除 EVP_CipherInit_ex 的最后一个参数外基本相同
	unsigned char key[16] = "15pb";
	EVP_CipherInit_ex(ctx, EVP_des_ede3_ecb(), NULL, key, nullptr, 0);

	EVP_CipherUpdate(ctx, des_string, &once_length, source_string, length);
	out_length += once_length;
	EVP_CipherFinal(ctx, des_string + once_length, &once_length);
	out_length += once_length;

	// 为加密后的数据添加空字符
	des_string[out_length] = 0;

	EVP_CIPHER_CTX_free(ctx);
	return out_length;
}