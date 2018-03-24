/**
 * This file is the DDDM assignment 1 - PHF.cpp
 *
 * @author Yuanzhong Xia (a1700831)
 */
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

int main(int argc, void **argv)
{
	cout << "This submission is used for testing what the input looks like:" << endl;
	string in;
	int counter = 1;
	while (getline(cin, in))
	{
		cout << "Line #" << counter++ << ": " << in << endl;
	}

	cout << "Looping through all arguments:" << endl;
	for (int i = 0; i < argc; i ++)
	{
		cout << "Arg #" << i + 1 << ": " << static_cast<char *>(argv[i]) << endl;
	}
	cout << "The end" << endl;

	// used for Visual Studio 2017 debugging output
#ifdef _MSC_VER
	system("pause");
#endif
    return 0;
}