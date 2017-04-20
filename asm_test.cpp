/*
 * asm_test.cpp
 *
 *  Created on: Apr 4, 2017
 *      Author: harry
 */

#include <iostream>
using namespace std;

int main()
{
	int a = 10, b = 20;

	// output a
	cout << "a=" << a << "\n";
	asm("mov $5, %eax;");
	asm("mov %%eax, %0;" : "=r"(a));
	cout << "a=" << a << "\n";

	// input b
	asm("mov %0, %%eax;" :: "r"(b));
	asm("sub $5, %eax;");
	asm("mov %%eax, %0;" : "=r"(a));
	cout << "a=" << a << "\n";

	asm(".byte 102,15,56,220,209");

	return 0;
}
