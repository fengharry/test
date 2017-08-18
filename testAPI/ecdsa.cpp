#include <iostream>
using namespace std;

#include <openssl/ecdsa.h>
#include <openssl/sha.h>

int main()
{	
	unsigned char msg[] = "hello";
	unsigned char rst[SHA256_DIGEST_LENGTH] = {0};
	unsigned char key[] = "test";
	unsigned int rst_len = 0;

    HMAC(EVP_sha256(), key, sizeof(key), msg, sizeof(msg), rst, &rst_len);

	cout << "hmac-sha256: rst_len=" << rst_len << "\n";
	for(int i=0;i<SHA256_DIGEST_LENGTH/8;i++)
	{
		for(int j=0;j<8;j++)
		{
			cout << hex << (int)rst[i+j] << " ";
		}
		cout << "\n";
	}

	return 0;
}
