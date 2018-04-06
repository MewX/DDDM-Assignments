/**
 * This file is the DDDM assignment 2 based on provided files.
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
#ifdef TESTING_DATACUBE_BUILDER
	// TODO: add testing codes here

#else

	// otherwise, this is for QUeryResponder program which takes two files input
	assert(argc >= 0);
	const string FILE_DATABASE = argv[1];
	const string FILE_QUERY = argv[2];
	// TODO:

#endif

	// used for Visual Studio 2017 debugging output
#ifdef _MSC_VER
	system("pause");
#endif
    return 0;
}
