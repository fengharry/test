#include <iostream>
using namespace std;

#if defined(os_windows)
// src is in memory, dest is register
#define ASMs(op, src, dest) __asm{op dest, src}
// src is register, dest is in memory
#define ASMd(op, src, dest) __asm{op dest, srci}
// both src and dest are in register
#define ASMn(op, src, dest) __asm{op dest, src}

#define ALIGN(X) __declspec(align(X))
#endif

#if defined(os_linux)
#define ADD_QUOTE(...) #__VA_ARGS__
#define ASMs(op, src, dest) asm(ADD_QUOTE(op %0, %%dest) :: "m"(src))
#define ASMd(op, src, dest) asm(ADD_QUOTE(op %%src, %0) :: "m"(dest))
#define ASMn(op, src, dest) asm(ADD_QUOTE(op %src, %dest))

#define ALIGN(X)
#endif

int main()
{
	ALIGN(16) unsigned char input[16], key[16], output[16];
	for(int i=0;i<16;i++)
	{
		input[i] = key[i] = output[i] = i;
	}

	ASMs(movdqa, input, xmm0);
	ASMs(movdqa, key, xmm1);
	ASMn(aesenc, xmm1, xmm0);
	ASMd(movdqa, xmm0, output);

	// 6a 6b 5e 46 28 68 35 56 b8 d0 57 6a 2b 91 2f 53
	for(int i=0;i<16;i++)
	{
		cout << hex << (int)output[i] << " ";
	}
	cout << "\n";
}
