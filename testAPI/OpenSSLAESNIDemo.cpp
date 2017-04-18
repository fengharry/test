// OpenSSLAESNIDemo.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#pragma comment(lib, "libeay32.lib")

#define AES_NO_PADDING 0
#define AES128_KEY_LENGTH 16
#define AES_ENCRYPT 1
#define AES_DECRYPT 0

int aes_do_crypt( const EVP_CIPHER *cipher // EVP_CIPHER method what is defined in .\crypto\evp\evp.h, example EVP_aes_128_ecb(), etc.
                , FILE *in                 // Input file, user can do little change to use buffer instead of file here, but may add input parameter for input buffer length
				, FILE *out                // Ouput file, user can do little change to use buffer instead of file here, but may add input parameter for output buffer length
				, const unsigned char *key // AES key. Normally, AES key is generated dynamically as key of synchronized algorithm should be protected and hidden, but not use const value.
				, const unsigned char *iv  // Initialization Vector for dedicated AES work mode. It should be protected and hidden, but not use const value.
				, int do_encrypt           // AES work mode. 1: encrypt, 0: decrypt
				, int padding)             // AES padding. 1: padding, 0: no_padding
{
	/* Allow enough space in output buffer for additional block */
    unsigned char inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
    int inlen, outlen;
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();;

	memset(inbuf, 0x00, 1024);
	memset(outbuf, 0x00, 1024);
	inlen = outlen = 0;
	EVP_CipherInit(ctx, cipher, key, NULL, do_encrypt);
	EVP_CIPHER_CTX_set_padding(ctx, padding);
	
	while(1)
	{
		inlen = fread(inbuf, 1, 1024, in);
        if(inlen <= 0) break;
        if(!EVP_CipherUpdate(ctx, outbuf, &outlen, inbuf, inlen))
        {
			EVP_CIPHER_CTX_cleanup(ctx);
            return 0;
        }
        fwrite(outbuf, 1, outlen, out);
    }
    if(!EVP_CipherFinal(ctx, outbuf, &outlen))
    {
        EVP_CIPHER_CTX_cleanup(ctx);
        return 0;
    }
    fwrite(outbuf, 1, outlen, out);
    EVP_CIPHER_CTX_cleanup(ctx);
	OPENSSL_cleanse(inbuf, 1024);
	OPENSSL_cleanse(outbuf, 1024);

	EVP_CIPHER_CTX_free(ctx);

    return 1;
}

int main(int argc, char* argv[])
{
	//One example for AES 128bits (Nk=4, Nr=10), refer appendix C.1 in fips-197
	static unsigned char plainText[16] =
	{
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
	};

	static unsigned char aesKey[AES128_KEY_LENGTH] = 
	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};
    
	static unsigned char encryptedText[16] =
	{
		0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a
	};

	unsigned char* plainBuffer[16];
	unsigned char* encryptedBuffer[16];	
	memset(plainBuffer, 0x00, 16);
	memset(encryptedBuffer, 0x00, 16);
	int result;
	FILE *in1, *out1;

	in1 = fopen("in1", "wb+");
	result = fwrite(&plainText[0], 1, 16, in1);
	result = fclose(in1);

	in1 = fopen("in1", "rb+");
	out1 = fopen("out1", "wb+"); 
	aes_do_crypt(EVP_aes_128_ecb(), in1, out1, aesKey, NULL, AES_ENCRYPT, AES_NO_PADDING);
	fclose(in1);
	fclose(out1);

	out1 = fopen("out1", "rb+");
	fread(encryptedBuffer, 1, 16, out1);
	fclose(out1);

	if (!memcmp(encryptedBuffer, encryptedText, 16))
	{
		printf("AES Encrypt test OK!\n");
	}
	else
	{
		printf("AES Encrypt test failed!\n");
	}

	FILE *in2, *out2;

	in2 = fopen("in2", "wb+");	
	fwrite(encryptedText, 1, 16, in2);
	fclose(in2);

	in2 = fopen("in2", "rb+");
	out2 = fopen("out2", "wb+"); 
	aes_do_crypt(EVP_aes_128_ecb(), in2, out2, aesKey, NULL, AES_DECRYPT, AES_NO_PADDING);
	fclose(in2);
	fclose(out2);

	out2 = fopen("out2", "rb+");
	fread(plainBuffer, 1, 16, out2);	
	fclose(out2);

	if (!memcmp(plainBuffer, plainText, 16))
	{
		printf("AES Decrypt test OK!\n");
	}
	else
	{
		printf("AES Decrypt test failed!\n");
	}

	return 0;
}

