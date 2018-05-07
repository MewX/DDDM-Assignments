/**
 * This file is the DDDM assignment 2 based on provided files.
 *
 * @author Yuanzhong Xia (a1700831)
 */
// enabling this is treated as the invoker for DatacubeBuilder.cpp testing
//#define TESTING_DATACUBE_BUILDER

// this defines the local envrionemnt
#ifdef _MSC_VER
#include "DatacubeBuilder.h"
#define DATA_CUBE_TYPE DatacubeBuilder
#else
#include "DatacubeConstructor.h"
#define DATA_CUBE_TYPE DatacubeConstructor
#endif

#include <cstdlib>
#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

// type defines
typedef std::vector<std::pair<std::string, int>> CODE_TABLE;


// constants
const string QUERY_ALL = "ALL";
const string QUERY_QUESTION = "?";

const CODE_TABLE UNIVERSITY_CODE_TABLE({ {"UofA", 0}, {"UniSA", 1}, {"Flinders", 2}, {"Torrens", 3} });
const CODE_TABLE PROGRAM_CODE_TABLE({ {"PhD", 0}, {"MCS", 1}, {"MSE", 2}, {"MCI", 3}, {"BCS", 4}, {"BSE", 5} });
const CODE_TABLE TERM_CODE_TABLE({ {"s1/2016", 0}, {"s2/2016", 1}, {"s1/2017", 2}, {"s2/2017", 3}, {"s1/2018", 4} });
const CODE_TABLE NATIONALITY_CODE_TABLE({ {"Australia", 0}, {"Korea", 1}, {"China", 2}, {"India", 3}, {"Iran", 4}, {"SriLanka", 5} });
const CODE_TABLE SCHOLARSHIP_CODE_TABLE({ {"No", 0}, {"Yes", 1} });
const vector<CODE_TABLE> CODE_TABLES({ UNIVERSITY_CODE_TABLE, PROGRAM_CODE_TABLE, TERM_CODE_TABLE, NATIONALITY_CODE_TABLE, SCHOLARSHIP_CODE_TABLE });


/**
 * The split function for C++ string.
 */
vector<string> split(const string &s, const char delim)
{
	vector<string> elems;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
	{
		if (!item.empty())
		{
			elems.push_back(item);
		}
	}
	return elems;
}

/**
 * compare two strings in case insensitive mode
 */
bool caseInsensitiveStringCompare(std::string a, std::string b)
{
	if (a.size() != b.size()) {
        return false;
    }
    for (auto c1 = a.begin(), c2 = b.begin(); c1 != a.end(); ++c1, ++c2) {
        if (tolower(*c1) != tolower(*c2)) {
            return false;
        }
    }
    return true;
}

/**
 * look for the value from the input table
 */
int findValueFromKey(const CODE_TABLE &table, const std::string &key)
{
	for (const auto &ele : table)
	{
		if (caseInsensitiveStringCompare(ele.first, key))
		{
			return ele.second;
		}
	}
	assert(false); // should not run into here from the given input
}

/**
 * find value from all keys
 */
int findValueFromKey(const std::string& key)
{
	for (const auto &table : CODE_TABLES)
	{
		for (const auto &ele : table)
		{
			if (caseInsensitiveStringCompare(ele.first, key))
			{
				return ele.second;
			}
		}
	}
	assert(false); // should not run into here from the given input
}

/**
 * find key from given value from CODE_TABLE
 */
std::string findKeyFromValue(const CODE_TABLE &table, const int value)
{
	for (const auto &ele : table)
	{
		if (ele.second == value)
		{
			return ele.first;
		}
	}
	assert(false); // should not run into here from the given input
}

/**
 * do the sum query
 */
void sumQuery(DATA_CUBE_TYPE *c, vector<string> &fullQuery)
{
	int iuni, iprogram, iterm, ination, ischolar;
	iuni = iprogram = iterm = ination = ischolar = -1; // -1 means not testing this property

	// update index (ALL uses -1)
	if (fullQuery[1] != QUERY_ALL) iuni = findValueFromKey(UNIVERSITY_CODE_TABLE, fullQuery[1]);
	if (fullQuery[2] != QUERY_ALL) iprogram = findValueFromKey(PROGRAM_CODE_TABLE, fullQuery[2]);
	if (fullQuery[3] != QUERY_ALL) iterm = findValueFromKey(TERM_CODE_TABLE, fullQuery[3]);
	if (fullQuery[4] != QUERY_ALL) ination = findValueFromKey(NATIONALITY_CODE_TABLE, fullQuery[4]);
	if (fullQuery[5] != QUERY_ALL) ischolar = findValueFromKey(SCHOLARSHIP_CODE_TABLE, fullQuery[5]);

	int sum = 0;
	for (unsigned uni = 0; uni < UNIVERSITY_CODE_TABLE.size(); uni++)
	{
		for (unsigned program = 0; program < PROGRAM_CODE_TABLE.size(); program++)
		{
			for (unsigned term = 0; term < TERM_CODE_TABLE.size(); term++)
			{
				for (unsigned nation = 0; nation < NATIONALITY_CODE_TABLE.size(); nation++)
				{
					for (unsigned scholar = 0; scholar < SCHOLARSHIP_CODE_TABLE.size(); scholar++)
					{
						const int temp = c->uptnsCuboid[uni][program][term][nation][scholar];

						if ((iuni < 0 || iuni == uni)
							&& (iprogram < 0 || iprogram == program)
							&& (iterm < 0 || iterm == term)
							&& (ination < 0 || ination == nation)
							&& (ischolar < 0 || ischolar == scholar))
						{
							sum += temp;
						}
					}
				}
			}
		}
	}
	cout << sum << endl; // output the result for sum query
}

/**
 * do the top_k query
 */
void topkQuery(DATA_CUBE_TYPE *c, vector<string> &fullQuery, const unsigned numOfResult)
{
	const int NUM_ALL = -1, NUM_QUESTION = -2;
	int iuni, iprogram, iterm, ination, ischolar;
	iuni = iprogram = iterm = ination = ischolar = NUM_ALL; // -1 means not testing this property

	// update index (ALL uses -1)
	if (fullQuery[1] != QUERY_ALL && fullQuery[1] != QUERY_QUESTION) iuni = findValueFromKey(UNIVERSITY_CODE_TABLE, fullQuery[1]);
	if (fullQuery[2] != QUERY_ALL && fullQuery[2] != QUERY_QUESTION) iprogram = findValueFromKey(PROGRAM_CODE_TABLE, fullQuery[2]);
	if (fullQuery[3] != QUERY_ALL && fullQuery[3] != QUERY_QUESTION) iterm = findValueFromKey(TERM_CODE_TABLE, fullQuery[3]);
	if (fullQuery[4] != QUERY_ALL && fullQuery[4] != QUERY_QUESTION) ination = findValueFromKey(NATIONALITY_CODE_TABLE, fullQuery[4]);
	if (fullQuery[5] != QUERY_ALL && fullQuery[5] != QUERY_QUESTION) ischolar = findValueFromKey(SCHOLARSHIP_CODE_TABLE, fullQuery[5]);

	// update index for `?`
	if (fullQuery[1] == QUERY_QUESTION) iuni = NUM_QUESTION;
	if (fullQuery[2] == QUERY_QUESTION) iprogram = NUM_QUESTION;
	if (fullQuery[3] == QUERY_QUESTION) iterm = NUM_QUESTION;
	if (fullQuery[4] == QUERY_QUESTION) ination = NUM_QUESTION;
	if (fullQuery[5] == QUERY_QUESTION) ischolar = NUM_QUESTION;

	map<string, int> result;
	for (unsigned uni = 0; uni < UNIVERSITY_CODE_TABLE.size(); uni++)
	{
		for (unsigned program = 0; program < PROGRAM_CODE_TABLE.size(); program++)
		{
			for (unsigned term = 0; term < TERM_CODE_TABLE.size(); term++)
			{
				for (unsigned nation = 0; nation < NATIONALITY_CODE_TABLE.size(); nation++)
				{
					for (unsigned scholar = 0; scholar < SCHOLARSHIP_CODE_TABLE.size(); scholar++)
					{
						const int temp = c->uptnsCuboid[uni][program][term][nation][scholar];

						if ((iuni < 0 || iuni == uni)
							&& (iprogram < 0 || iprogram == program)
							&& (iterm < 0 || iterm == term)
							&& (ination < 0 || ination == nation)
							&& (ischolar < 0 || ischolar == scholar))
						{
							// find map key
							string key;
							if (iuni == NUM_QUESTION) key = findKeyFromValue(UNIVERSITY_CODE_TABLE, uni);
							else if (iprogram == NUM_QUESTION) key = findKeyFromValue(PROGRAM_CODE_TABLE, program);
							else if (iterm == NUM_QUESTION) key = findKeyFromValue(TERM_CODE_TABLE, term);
							else if (ination == NUM_QUESTION) key = findKeyFromValue(NATIONALITY_CODE_TABLE, nation);
							else if (ischolar == NUM_QUESTION) key = findKeyFromValue(SCHOLARSHIP_CODE_TABLE, scholar);
							else assert(false); // impossible to run into here

							// update record
							if (temp != 0)
							{
								if (result.count(key) == 0) result[key] = temp;
								else result[key] += temp;
							}
						}
					}
				}
			}
		}
	}

	// output
	if (result.empty())
	{
		cout << "NULL" << endl;
	}
	else
	{
		// sort the map
		vector<string> outList;
		for (const auto key : result) outList.push_back(key.first);

		sort( outList.begin( ), outList.end(), [&result](const string &lhs, const string &rhs)
		{
			if (result[lhs] != result[rhs]) return result[lhs] > result[rhs];
			return findValueFromKey(lhs) < findValueFromKey(rhs);
		});

		// print result
		const unsigned upperBound = outList.size() < numOfResult ? outList.size() : numOfResult;
		for (unsigned i = 0; i < upperBound; i ++)
		{
			if (result[outList[i]] == 0) break;
			if (i == 0) cout << outList[i];
			else cout << " " << outList[i];
		}
		cout << endl;
	}
}

/**
 * run all query in one place
 */
void runQuery(DATA_CUBE_TYPE *c, const char* queryFileName)
{
	// read the whole query.txt file
	const string QUERY_FILE_NAME = queryFileName;
	string line;
	vector<string> lines;
	ifstream fileDatabase(QUERY_FILE_NAME);
	while (getline(fileDatabase, line))
	{
		if (line.empty()) continue;
		lines.push_back(line);
	}
	fileDatabase.close();

	// each query
	for (unsigned i = 0; i < lines.size(); i++)
	{
		auto tokens = split(lines[i], '\t');

		// pre-process
		for (unsigned j = 1; j < tokens.size(); j++)
		{
			if (!tokens[j].compare(0, QUERY_ALL.length(), QUERY_ALL))
			{
				tokens[j] = QUERY_ALL;
			}
			else if (!tokens[j].compare(0, QUERY_QUESTION.length(), QUERY_QUESTION))
			{
				tokens[j] = QUERY_QUESTION;
			}
		}

		// separate query types
		if (tokens[0] == "SUM")
		{
			// the sum query
			sumQuery(c, tokens);
		}
		else
		{
			// the top_k query
			topkQuery(c, tokens, tokens[0][tokens[0].length() - 1] - '0');
		}
	}
}


/**
 * the main entry of the query program
 */
int main(int argc, char **argv)
{
	// taking two input files
	assert(argc >= 0);

	// load data and generate data cube
#ifdef _MSC_VER
	DatacubeBuilder *datacube = new DatacubeBuilder;
	datacube->buildCuboid(argv[1]);
#else
	DatacubeConstructor *datacube = new DatacubeConstructor();
	datacube->constructCuboid(argv[1]);
#endif


	// run queries
	runQuery(datacube, argv[2]);

	// used for Visual Studio 2017 debugging output
#ifdef _MSC_VER
	system("pause");
#endif
    return 0;
}
