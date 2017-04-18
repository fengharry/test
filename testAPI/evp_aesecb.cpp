#include <iostream>
#include <openssl/evp.h>

using namespace std;

int main()
{	
	unsigned char key[32] = {1};
    unsigned char iv[16] = {0};
    unsigned char inStr[] = "this is test string";
    int inLen = sizeof(inStr);
    int encLen = 0;
    int outlen = 0;
    unsigned char encData[1024];
    
    cout <<"source: " << inStr << "\n";
    
    //加密
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    
    EVP_CipherInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, iv, 1);
    EVP_CipherUpdate(ctx, encData, &outlen, inStr, inLen);
    encLen = outlen;
    EVP_CipherFinal(ctx, encData+outlen, &outlen);
    encLen += outlen;
    EVP_CIPHER_CTX_free(ctx);
    
    
    //解密
    int decLen = 0;
    outlen = 0;
    unsigned char decData[1024];
    EVP_CIPHER_CTX *ctx2;
    ctx2 = EVP_CIPHER_CTX_new();
    EVP_CipherInit_ex(ctx2, EVP_aes_256_ecb(), NULL, key, iv, 0);
    EVP_CipherUpdate(ctx2, decData, &outlen, encData, encLen);
    decLen = outlen;
    EVP_CipherFinal(ctx2, decData+outlen, &outlen);
    decLen += outlen;
    EVP_CIPHER_CTX_free(ctx2);
    
    decData[decLen] = '\0';
    cout <<"decrypt: " << decData << "\n";
}
