/**
 * This file is the DDDM assignment 2 based on provided files.
 *
 * @author Yuanzhong Xia (a1700831)
 */
// enabling this is treated as the invoker for DatacubeBuilder.cpp testing
//#define TESTING_DATACUBE_BUILDER

#include "DatacubeBuilder.h"

#include <cstdlib>
#include <string>
#include <cassert>
using namespace std;

int main(int argc, char **argv)
{
	// taking two input files
	assert(argc >= 0);
	const string FILE_DATABASE = argv[1];
	const string FILE_QUERY = argv[2];

	// load data and generate data cube (set breakpoint here to debug the data cube
	DatacubeBuilder datacube;
	datacube.buildCuboid(FILE_DATABASE.c_str());

	// TODO: run queries


	// used for Visual Studio 2017 debugging output
#ifdef _MSC_VER
	system("pause");
#endif
    return 0;
}
