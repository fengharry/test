#include <iostream>
#include <openssl/aes.h>  

using namespace std;

#define DATA_SIZE 16
  
int main()  
{  
    unsigned char InputData[DATA_SIZE], EncryptData[DATA_SIZE], DecryptData[DATA_SIZE];  
    unsigned char iv[16];  
    unsigned char key[16];  
    AES_KEY AesKey;  

	cout << "InputData: ";
    for(int i=0; i<16; i++)  
    {  
    	InputData[i] = i;
		cout << hex << (int)InputData[i] << " ";
    }  
	cout << "\n";
      
	// encrypt
    for(int i=0; i<16; i++)  
    {  
		iv[i] = i;
        key[i] = i;  
    }  
	
    AES_set_encrypt_key(key, 128, &AesKey);
    AES_cbc_encrypt(InputData, EncryptData, DATA_SIZE, &AesKey, iv, AES_ENCRYPT);  
	cout << "EncryptData: ";
    for(int i=0; i<16; i++)  
    {  
    	cout << hex << (int)EncryptData[i] << " ";
	}  
	cout << "\n";
	
	cout << "iv after encrypt: ";
	for(int i=0; i<16; i++)
	{
		cout << hex << (int)iv[i] << " ";
	}
	cout << "\n";
      
    // decrypt
    for(int i=0; i<16; i++)  
    {  
		iv[i] = i;
    }  
    AES_set_decrypt_key(key, 128, &AesKey);
    AES_cbc_encrypt(EncryptData, DecryptData, DATA_SIZE, &AesKey, iv, AES_DECRYPT);  
	cout << "DecryptData: ";
    for(int i=0; i<16; i++)  
    {  
    	cout << hex << (int)DecryptData[i] << " ";
    }  
	cout << "\n";

	cout << "iv after decrypt: ";
	for(int i=0; i<16; i++)
	{
		cout << hex << (int)iv[i] << " ";
	}
	cout << "\n";
     
    return 0;  
}  
