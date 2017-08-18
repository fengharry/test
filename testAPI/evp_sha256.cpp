#include <iostream>
using namespace std;

#include <openssl/evp.h>
#include <openssl/sha.h>

int main()
{	
	unsigned char msg[] = "hello";
	unsigned char rst[SHA256_DIGEST_LENGTH] = {0};

    EVP_Digest(msg, sizeof(msg), rst, NULL, EVP_sha256(), NULL);

	cout << "sha256:\n";
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
