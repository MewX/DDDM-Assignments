//
// This code helps you test your query algo 
// without concerning the correctness of your datacube algo
// You DO NOT need to call your DatacubeBuilder here because it may not be correct
//
// Author: Zhigang Lu

#include <iostream>


#include "DatacubeConstructor.h"

using namespace std;


// main() func

int main(int argc, char *argv[]) {

	// calling our datacube constructor
	// DO NOT CALL THE FOLLOWING WHEN TESTING YOUR FIRST ALGORITHM
	DatacubeConstructor *constructor = new DatacubeConstructor();
	constructor->constructCuboid(argv[1]);


	//argv[2] would be the input file of query


	//Output of TOP_K query follows the order of number of enrolment
	//i.e., max -> min

	
	return 0;
}
