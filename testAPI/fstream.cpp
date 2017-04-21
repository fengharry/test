#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		cout << "usage: " <<  argv[0] << " file\n";
	}

	fstream file(argv[1], ios::in|ios::out|ios::binary);
	file.seekp(3, ios::beg);
	file.write("ab", 2);
	file.close();

	return 0;
}
