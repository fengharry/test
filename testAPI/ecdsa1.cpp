#include <iostream>
using namespace std;

#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/sha.h>

int main()
{	
	unsigned char msg[] = "hello";
	unsigned char *rst = NULL;
	unsigned int rst_len = 0;

	EC_KEY *ecdsa_key = NULL;
	ECDSA_SIG *ecdsa_sig = NULL;
	const BIGNUM *sig_r, *sig_s;
	int r_len = 0;
	int s_len = 0;

	ecdsa_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	if(NULL == ecdsa_key)
	{
		cout << "can't allocate ecc key!\n";
		return -1;
	}
	EC_KEY_generate_key(ecdsa_key);

	ecdsa_sig = ECDSA_do_sign(msg, sizeof(msg), ecdsa_key);
	if(NULL == ecdsa_sig)
	{
		cout << "can't sign the message!\n";
		return -1;
	}
	ECDSA_SIG_get0(ecdsa_sig, &sig_r, &sig_s);

	r_len = BN_num_bytes(sig_r);
	s_len = BN_num_bytes(sig_s);
	cout << "r_len=" << r_len << " s_len=" << s_len << "\n";
	rst_len = r_len + s_len;
	rst = new unsigned char[rst_len];
	if(NULL == rst)
	{
		cout << "can't allocate result buffer!\n";
		return -1;
	}
	BN_bn2bin(sig_r, rst);
	BN_bn2bin(sig_s, rst+r_len);

	ECDSA_SIG_free(ecdsa_sig);
	EC_KEY_free(ecdsa_key);

	cout << "ecdsa: rst_len=" << rst_len << "\n";
	for(int i=0;i<rst_len/8;i++)
	{
		for(int j=0;j<8;j++)
		{
			cout << hex << (int)rst[i+j] << " ";
		}
		cout << "\n";
	}

	return 0;
}
