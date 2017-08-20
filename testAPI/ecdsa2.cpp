#include <iostream>
using namespace std;

#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>

int main()
{	
	EC_KEY *ecdsa_key = NULL;
	EC_POINT *pub_key = NULL;
	const EC_GROUP *group = NULL;

	ecdsa_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	if(NULL == ecdsa_key)
	{
		cout << "can't allocate ecc key!\n";
		return -1;
	}
	
	group = EC_KEY_get0_group(ecdsa_key);
	pub_key = EC_POINT_new(group);
	EC_POINT_hex2point(group,
			    "04fb1a64460482d8598b2ed69c7ca04627f08ae648a01b31eafe3816ca25f8626867d2cdf0d7f4509196bc146b0c2e9d50d273313a84829536638119c557e75475", pub_key, NULL);
	EC_KEY_set_public_key(ecdsa_key, pub_key);

	if(!EC_KEY_check_key(ecdsa_key))
	{
		cout << "ECDSA public key check fail!\n";
	}
	else
	{
		cout << "ECDSA public key check ok!\n";
	}

	return 0;
}
