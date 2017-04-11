#include <iostream>
#include<sys/time.h>
#include<unistd.h>

using namespace std;

int main()
{
	struct  timeval    tv1, tv2;
	struct  timezone   tz1, tz2;

	gettimeofday(&tv1,&tz1);
	int start = tv1.tv_sec*1000000 + tv1.tv_usec;

	int sum = 0;
	for(int i=0;i<100000;i++)
	{
    	sum += i;
	}
	cout << "sum=" << sum << "\n";

	gettimeofday(&tv2,&tz2);
	int end = tv2.tv_sec*1000000 + tv2.tv_usec;

	cout << end - start << " us" << "\n";
	
	return 0;
}
