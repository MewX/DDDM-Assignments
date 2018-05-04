/**
 * This file is the DDDM assignment 2.
 *
 * @author Yuanzhong Xia (a1700831)
 */
#include "DatacubeBuilder.h"
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

/**
 * the constructor
 */
DatacubeBuilder::DatacubeBuilder() :
	UNIVERSITY_CODE_TABLE({ {"UofA", 0}, {"UniSA", 1}, {"Flinders", 2}, {"Torrens", 3} }),
	PROGRAM_CODE_TABLE({ {"PhD", 0}, {"MCS", 1}, {"MSE", 2}, {"MCI", 3}, {"BCS", 4}, {"BSE", 5} }),
	TERM_CODE_TABLE({ {"s1/2016", 0}, {"s2/2016", 1}, {"s1/2017", 2}, {"s2/2017", 3}, {"s1/2018", 4} }),
	NATIONALITY_CODE_TABLE({ {"Australia", 0}, {"Korea", 1}, {"China", 2}, {"India", 3}, {"Iran", 4}, {"SriLanka", 5} }),
	SCHOLARSHIP_CODE_TABLE({ {"No", 0}, {"Yes", 1} }),
	CODE_TABLES({ UNIVERSITY_CODE_TABLE, PROGRAM_CODE_TABLE, TERM_CODE_TABLE, NATIONALITY_CODE_TABLE, SCHOLARSHIP_CODE_TABLE })
{
}

/**
 * look for the value from the input table
 */
int DatacubeBuilder::findValueFromKey(const CODE_TABLE &table, const std::string &key) const
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

int DatacubeBuilder::findValueFromKey(const std::string& key) const
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

std::string DatacubeBuilder::findKeyFromValue(const CODE_TABLE &table, const int value) const
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

bool DatacubeBuilder::caseInsensitiveStringCompare(std::string a, std::string b) const
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
 * The split function for C++ string.
 */
vector<string> DatacubeBuilder::split(const string &s, const char delim) const
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
 * the required function
 */
void DatacubeBuilder::buildCuboid(const char *dbFileName)
{
	// read all lines from the database file (column order)
	// University	Program	Term	Nationality	Scholarship	Enrolment
	const string DB_FILE_NAME = dbFileName;
	string line;
	vector<string> lines;
	ifstream fileDatabase(DB_FILE_NAME);
	while (getline(fileDatabase, line))
	{
		if (line.empty()) continue;
		lines.push_back(line);
	}
	fileDatabase.close();

	// build from 5-D cuboid (skipping the first column name)
	build5dCuboid(lines);

	// build all other cuboids based on 5-D cuboid
	buildAllLowerCuboids();
}

void DatacubeBuilder::build5dCuboid(const vector<string> lines)
{
	for (unsigned i = 1; i < lines.size(); i ++)
	{
		// University, Program, Term, Nationality, Scholarship, Enrolment
		auto tokens = split(lines[i], '\t');
		const int uni = findValueFromKey(UNIVERSITY_CODE_TABLE, tokens[0]);
		const int program = findValueFromKey(PROGRAM_CODE_TABLE, tokens[1]);
		const int term = findValueFromKey(TERM_CODE_TABLE, tokens[2]);
		const int nation = findValueFromKey(NATIONALITY_CODE_TABLE, tokens[3]);
		const int scholarship = findValueFromKey(SCHOLARSHIP_CODE_TABLE, tokens[4]);
		const int enrolment = atoi(tokens[5].c_str());

		// set it
		uptnsCuboid[uni][program][term][nation][scholarship] = enrolment;
	}
}

void DatacubeBuilder::buildAllLowerCuboids()
{
	for (unsigned uni = 0; uni < UNIVERSITY_CODE_TABLE.size(); uni ++)
	{
		for (unsigned program = 0; program < PROGRAM_CODE_TABLE.size(); program ++)
		{
			for (unsigned term = 0; term < TERM_CODE_TABLE.size(); term ++)
			{
				for (unsigned nation = 0; nation < NATIONALITY_CODE_TABLE.size(); nation ++)
				{
					for (unsigned scholar = 0; scholar < SCHOLARSHIP_CODE_TABLE.size(); scholar ++)
					{
						const int temp = uptnsCuboid[uni][program][term][nation][scholar];

						// 4d cuboids
						uptnCuboid[uni][program][term][nation] += temp;
						uptsCuboid[uni][program][term][scholar] += temp;
						upnsCuboid[uni][program][nation][scholar] += temp;
						utnsCuboid[uni][term][nation][scholar] += temp;
						ptnsCuboid[program][term][nation][scholar] += temp;

						// 3d cuboids
						uptCuboid[uni][program][term] += temp;
						upnCuboid[uni][program][nation] += temp;
						upsCuboid[uni][program][scholar] += temp;
						utnCuboid[uni][term][nation] += temp;
						utsCuboid[uni][term][scholar] += temp;
						unsCuboid[uni][nation][scholar] += temp;
						ptnCuboid[program][term][nation] += temp;
						ptsCuboid[program][term][scholar] += temp;
						pnsCuboid[program][nation][scholar] += temp;
						tnsCuboid[term][nation][scholar] += temp;

						// 2d cuboids
						upCuboid[uni][program] += temp;
						utCuboid[uni][term] += temp;
						unCuboid[uni][nation] += temp;
						usCuboid[uni][scholar] += temp;
						ptCuboid[program][term] += temp;
						pnCuboid[program][nation] += temp;
						psCuboid[program][scholar] += temp;
						tnCuboid[term][nation] += temp;
						tsCuboid[term][scholar] += temp;
						nsCuboid[nation][scholar] += temp;

						// 1d cuboids
						uCuboid[uni] += temp;
						pCuboid[program] += temp;
						tCuboid[term] += temp;
						nCuboid[nation] += temp;
						sCuboid[scholar] += temp;

						// all
						all += temp;
					}
				}
			}
		}
	}
}

void DatacubeBuilder::sumQuery(std::vector<std::string> &fullQuery) const
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
						const int temp = uptnsCuboid[uni][program][term][nation][scholar];

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

void DatacubeBuilder::topkQuery(std::vector<std::string> &fullQuery, const unsigned numOfResult) const
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
						const int temp = uptnsCuboid[uni][program][term][nation][scholar];

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

		sort( outList.begin( ), outList.end(), [&result, this](const string &lhs, const string &rhs)
		{
			if (result[lhs] != result[rhs]) return result[lhs] > result[rhs];
			return findValueFromKey(lhs) < findValueFromKey(rhs);
		});

		// print result
		for (unsigned i = 0; i < min(outList.size(), numOfResult); i ++)
		{
			if (result[outList[i]] == 0) break;
			if (i == 0) cout << outList[i];
			else cout << " " << outList[i];
		}
		cout << endl;
	}
}

void DatacubeBuilder::runQuery(const char* queryFileName) const
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
	for (unsigned i = 0; i < lines.size(); i ++)
	{
		auto tokens = split(lines[i], '\t');

		// pre-process
		for (unsigned j = 1; j < tokens.size(); j ++)
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
			sumQuery(tokens);
		}
		else
		{
			// the top_k query
			topkQuery(tokens, tokens[0][tokens[0].length() - 1] - '0');
		}
	}

}
