#include <iostream>
#include <chrono>

using namespace std;
 
int main()
{
	cout << "system_clock" << endl;
	cout << chrono::system_clock::period::num << endl;
	cout << chrono::system_clock::period::den << endl;
	cout << "steady = " << boolalpha << chrono::system_clock::is_steady << endl << endl;

	cout << "high_resolution_clock" << endl;
	cout << chrono::high_resolution_clock::period::num << endl;
	cout << chrono::high_resolution_clock::period::den << endl;
	cout << "steady = " << boolalpha << chrono::high_resolution_clock::is_steady << endl << endl;

	cout << "steady_clock" << endl;
	cout << chrono::steady_clock::period::num << endl;
	cout << chrono::steady_clock::period::den << endl;
	cout << "steady = " << boolalpha << chrono::steady_clock::is_steady << endl << endl;

	auto start = chrono::steady_clock::now();
	int sum = 0;
	for(int i=0;i<1000000;i++)
	{
		sum += i;
	}
	cout << "sum=" << sum << "\n";
	auto end = chrono::steady_clock::now();
	auto diff = end -start;

	cout << chrono::duration <double, milli> (diff).count() << " ms" << "\n";

	return 0;    
}
