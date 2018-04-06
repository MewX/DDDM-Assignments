/**
 * This file is the DDDM assignment 2 based on provided files.
 *
 * @author Yuanzhong Xia (a1700831)
 */
#ifndef _DATACUBE_BUILDER_H_
#define _DATACUBE_BUILDER_H_

class DatacubeBuilder {
public:
	//5-D Cuboid
	//University-Program-Term-Nationality-Scholarship
	int uptnsCuboid[4][6][5][6][2];
	//4-D Cuboid
	int uptnCuboid[4][6][5][6];
	int uptsCuboid[4][6][5][2];
	int upnsCuboid[4][6][6][2];
	int utnsCuboid[4][5][6][2];
	int ptnsCuboid[6][5][6][2];
	//3-D Cuboid
	int uptCuboid[4][6][5];
	int upnCuboid[4][6][6];
	int upsCuboid[4][6][2];
	int utnCuboid[4][5][6];
	int utsCuboid[4][5][2];
	int unsCuboid[3][6][2];
	int ptnCuboid[6][5][6];
	int ptsCuboid[6][5][2];
	int pnsCuboid[6][6][2];
	int tnsCuboid[5][6][2];
	//2-D Cuboid
	int upCuboid[4][6];
	int utCuboid[4][5];
	int unCuboid[4][6];
	int usCuboid[4][2];
	int ptCuboid[6][5];
	int pnCuboid[6][6];
	int psCuboid[6][2];
	int tnCuboid[5][6];
	int tsCuboid[5][2];
	int nsCuboid[6][2];
	//1-D Cuboid
	int uCuboid[4];
	int pCuboid[6];
	int tCuboid[5];
	int nCuboid[6];
	int sCuboid[2];
	//0-D Cuboid
	int all;

	// constructor
	DatacubeBuilder();

	// Our driver will call this func to test your data cube,
	// Please make it as a public function.
	// The parameter to this function will be argv[1] to the main() in our driver
	// i.e. the database.txt
	void buildCuboid(char *);
};

#endif
