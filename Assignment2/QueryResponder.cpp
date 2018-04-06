/**
 * This file is the DDDM assignment 2.
 *
 * @author Yuanzhong Xia (a1700831)
 */
// enabling this is treated as the invoker for DatacubeBuilder.cpp testing
//#define TESTING_DATACUBE_BUILDER

#ifdef TESTING_DATACUBE_BUILDER
#include "DatacubeBuilder.h"

#else
#include <cstdlib>
#include <string>
#include <cassert>
using namespace std;

#endif

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		// TODO:

		return 0;
	}

	// otherwise, this is for QUeryResponder program which takes two files input
	const string FILE_DATABASE = argv[1];
	const string FILE_QUERY = argv[2];
	// TODO:

	// used for Visual Studio 2017 debugging output
#ifdef _MSC_VER
	system("pause");
#endif
    return 0;
}
