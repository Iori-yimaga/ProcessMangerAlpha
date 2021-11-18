#include "pch.h"
#include <openssl\pem.h>
#include <openssl\rand.h>
#include <openssl\md5.h>
#include <openssl\bio.h>
#include <openssl\evp.h>
#include <openssl\applink.c>
#pragma comment(lib, "libcrypto.lib")
#include "EnDeCrypt.h"

// ������Կ��
void generateRSAKEY() {
	RSA* keypair = RSA_generate_key(
		1024,    // ��Կ����
		RSA_F4,  // ��Կָ��
		NULL,
		NULL
	);
	// �����ɵ���Կ���ж�ȡ˽Կ���ڴ����
	BIO* pri = BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
	// ��ȡ��Կ���Ȳ�������ռ���б���
	size_t priLen = BIO_pending(pri);
	char* priKey = (char*)calloc(priLen + 1, sizeof(char));
	BIO_read(pri, priKey, priLen);
	// �����ɵ�˽Կд�뵽ָ�����ļ���
	FILE* priFile = NULL;
	if (fopen_s(&priFile, PRI_KEY, "w") == NULL) {
		if (priKey && priFile) {
			fputs(priKey, priFile);
			fclose(priFile);
		}
	}
	// �����ɵ���Կ���ж�ȡ��Կ���ڴ����
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
	// �ͷ���Դ
	RSA_free(keypair);
	BIO_free_all(pub);
	BIO_free_all(pri);
	free(priKey);
	free(pubKey);
}

// ��ȡ��Կ
RSA* getPubKey() {
	// �򿪹�Կ�ļ�
	FILE* pubFile = NULL;
	if (fopen_s(&pubFile, PUB_KEY, "r") == NULL) {
		// ��ָ���ļ��ж�ȡ��Կ
		RSA* rsa = PEM_read_RSAPublicKey(pubFile, NULL, NULL, NULL);
		if (pubFile) fclose(pubFile);
		return rsa;
	}
	return nullptr;
}

// ��ȡ˽Կ
RSA* getPriKEy() {
	// ��˽Կ�ļ�
	FILE* priFile = NULL;
	if (fopen_s(&priFile, PRI_KEY, "r") == NULL) {
		// ��ָ���ļ��ж�ȡ��Կ
		RSA* rsa = PEM_read_RSAPrivateKey(priFile, NULL, NULL, NULL);
		if (priFile) fclose(priFile);
		return rsa;
	}
	return nullptr;
}

// ����
BYTE* RSAEncrypt(BYTE* data, RSA* rsa) {
	int rsaLen = RSA_size(rsa);
	BYTE* encrypt = (BYTE*)malloc(rsaLen);
	RSA_public_encrypt(117, data, encrypt, rsa, RSA_PKCS1_PADDING);

	return encrypt;
}

// ����
BYTE* RSADecrypt(BYTE* data, RSA* rsa) {
	int rsaLen = RSA_size(rsa);
	BYTE* decrypt = (BYTE*)malloc(rsaLen);
	RSA_private_decrypt(rsaLen, data, decrypt, rsa, RSA_PKCS1_PADDING);

	return decrypt;
}

// MD5����
int MD5Encrypt(const void* data, size_t len, unsigned char* md5) {
	// ��ʼ������ md5 ��Ϣ�Ľṹ��
	MD5_CTX ctx = { 0 };
	MD5_Init(&ctx);
	// ����Ҫ��������ݴ��뵽��Ӧ�Ľṹ��
	MD5_Update(&ctx, data, len);
	// �ӽṹ�л�ȡ�����Ľ��
	MD5_Final(md5, &ctx);
	return 0;
}

// ����ֵ�Ǽ��ܺ�����ĳ��ȣ��������ģ����ĳ��Ⱥͱ������ĵĻ�����
// AES ����
int evpEncrypt(unsigned char* source_string, unsigned char* des_string, int length){
	// ����һ��ͨ�üӽ��ܵĶ���������䷽ʽ
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx, 1);

	// once_length ��һ�μ��ܵĳ���, out_length ���ܺ����ĵĳ��� 
	int once_length = 0, out_length = 0;

	// ���ü���ʱʹ�õ��㷨+����ģʽ�� key �����ĵ����ڶ�����������
	// ����ָ����ʼ�����������һ�������� 1 ��ʾ���ܣ��������
	unsigned char key[16] = "15pb";
	EVP_CipherInit_ex(ctx, EVP_des_ede3_ecb(), NULL, key, nullptr, 1);

	// ʹ�� EVP_CipherUpdate + EVP_CipherFinal �����������
	EVP_CipherUpdate(ctx, des_string, &once_length, source_string, length);
	out_length += once_length;
	EVP_CipherFinal(ctx, des_string + once_length, &once_length);
	out_length += once_length;

	// ������󲢷��س���
	EVP_CIPHER_CTX_free(ctx);
	return out_length;
}

// AES����
int evpDecrypt(unsigned char* source_string, unsigned char* des_string, int length){
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx, 1);

	int once_length = 0, out_length = 0;

	// ���ܵĴ���ͼ��ܳ� EVP_CipherInit_ex �����һ�������������ͬ
	unsigned char key[16] = "15pb";
	EVP_CipherInit_ex(ctx, EVP_des_ede3_ecb(), NULL, key, nullptr, 0);

	EVP_CipherUpdate(ctx, des_string, &once_length, source_string, length);
	out_length += once_length;
	EVP_CipherFinal(ctx, des_string + once_length, &once_length);
	out_length += once_length;

	// Ϊ���ܺ��������ӿ��ַ�
	des_string[out_length] = 0;

	EVP_CIPHER_CTX_free(ctx);
	return out_length;
}