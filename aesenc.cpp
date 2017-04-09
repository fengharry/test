#include <iostream>
using namespace std;

int main()
{
	unsigned char input[16], key[16], output[16];
	for(int i=0;i<16;i++)
	{
		input[i] = key[i] = output[i] = i;
	}

	asm("movdqa %0, %%xmm0" :: "m"(input));
	asm("movdqa %0, %%xmm1" :: "m"(key));
	asm("aesenc %xmm0, %xmm1");
	asm("movdqa %%xmm1, %0" : "=m"(output));

	for(int i=0;i<16;i++)
	{
		cout << hex << (int)output[i] << " ";
	}
	cout << "\n";
}
