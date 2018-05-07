/**
 * This file is the DDDM assignment 2 based on provided files.
 *
 * @author Yuanzhong Xia (a1700831)
 */
#ifndef _DATACUBE_BUILDER_H_
#define _DATACUBE_BUILDER_H_
#include <string>
#include <vector>
#include <map>

// types
typedef std::vector<std::pair<std::string, int>> CODE_TABLE;

class DatacubeBuilder
{
private:
	const CODE_TABLE UNIVERSITY_CODE_TABLE;
	const CODE_TABLE PROGRAM_CODE_TABLE;
	const CODE_TABLE TERM_CODE_TABLE;
	const CODE_TABLE NATIONALITY_CODE_TABLE;
	const CODE_TABLE SCHOLARSHIP_CODE_TABLE;

	int findValueFromKey(const CODE_TABLE& table, const std::string& key) const;
	static bool caseInsensitiveStringCompare(std::string a, std::string b);

	void build5dCuboid(const std::vector<std::string> lines);
	void buildAllLowerCuboids();

public:
	//5-D Cuboid
	//University-Program-Term-Nationality-Scholarship
	int uptnsCuboid[4][6][5][6][2] = {{{{{0}}}}};
	//4-D Cuboid
	int uptnCuboid[4][6][5][6] = {{{{0}}}};
	int uptsCuboid[4][6][5][2] = {{{{0}}}};
	int upnsCuboid[4][6][6][2] = {{{{0}}}};
	int utnsCuboid[4][5][6][2] = {{{{0}}}};
	int ptnsCuboid[6][5][6][2] = {{{{0}}}};
	//3-D Cuboid
	int uptCuboid[4][6][5] = {{{0}}};
	int upnCuboid[4][6][6] = {{{0}}};
	int upsCuboid[4][6][2] = {{{0}}};
	int utnCuboid[4][5][6] = {{{0}}};
	int utsCuboid[4][5][2] = {{{0}}};
	int unsCuboid[3][6][2] = {{{0}}};
	int ptnCuboid[6][5][6] = {{{0}}};
	int ptsCuboid[6][5][2] = {{{0}}};
	int pnsCuboid[6][6][2] = {{{0}}};
	int tnsCuboid[5][6][2] = {{{0}}};
	//2-D Cuboid
	int upCuboid[4][6] = {{0}};
	int utCuboid[4][5] = {{0}};
	int unCuboid[4][6] = {{0}};
	int usCuboid[4][2] = {{0}};
	int ptCuboid[6][5] = {{0}};
	int pnCuboid[6][6] = {{0}};
	int psCuboid[6][2] = {{0}};
	int tnCuboid[5][6] = {{0}};
	int tsCuboid[5][2] = {{0}};
	int nsCuboid[6][2] = {{0}};
	//1-D Cuboid
	int uCuboid[4] = {0};
	int pCuboid[6] = {0};
	int tCuboid[5] = {0};
	int nCuboid[6] = {0};
	int sCuboid[2] = {0};
	//0-D Cuboid
	int all = 0;

	// constructor
	DatacubeBuilder();

	// Our driver will call this func to test your data cube,
	// Please make it as a public function.
	// The parameter to this function will be argv[1] to the main() in our driver
	// i.e. the database.txt
	void buildCuboid(const char*);

	std::vector<std::string> split(const std::string& s, const char delim) const;
};

#endif
