#include <iostream>
using namespace std;

#if defined(os_windows)
#define ASMs(op, src, dest) __asm op dest, src
#define ASMd(op, src, dest) asm(#op src, %0 :: "m"(dest))
#endif

#if defined(os_linux)
#define ASMs(op, src, dest) asm("op %0, dest" :: "m"(src))
#define ASMd(op, src, dest) asm("op src, %0" :: "m"(dest))
#endif

int main()
{
	unsigned char input[16], key[16], output[16];
	for(int i=0;i<16;i++)
	{
		input[i] = key[i] = output[i] = i;
	}

	//asm("movdqa %0, %%xmm0" :: "m"(input));
	ASMd(movdqa, input, xmm0);
	__asm movdqa input, xmm0;
	//asm("movdqa %0, %%xmm1" :: "m"(key));
	//asm("aesenc %xmm0, %xmm1");
	//asm("movdqa %%xmm1, %0" : "=m"(output));

	for(int i=0;i<16;i++)
	{
		cout << hex << (int)output[i] << " ";
	}
	cout << "\n";
}
