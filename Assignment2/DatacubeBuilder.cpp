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

using namespace std;

/**
 * the constructor
 */
DatacubeBuilder::DatacubeBuilder() :
	UNIVERSITY_CODE_TABLE({ {"UofA", 0}, {"UniSA", 1}, {"Flinders", 2}, {"Torrens", 3} }),
	PROGRAM_CODE_TABLE({ {"PhD", 0}, {"MCS", 1}, {"MSE", 2}, {"MCI", 3}, {"BCS", 4}, {"BSE", 5} }),
	TERM_CODE_TABLE({ {"s1/2016", 0}, {"s2/2016", 1}, {"s1/2017", 2}, {"s2/2017", 3}, {"s1/2018", 4} }),
	NATIONALITY_CODE_TABLE({ {"Australia", 0}, {"Korea", 1}, {"China", 2}, {"India", 3}, {"Iran", 4}, {"SriLanka", 5} }),
	SCHOLARSHIP_CODE_TABLE({ {"No", 0}, {"Yes", 1} })
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
	return NULL;
}

bool DatacubeBuilder::caseInsensitiveStringCompare(std::string a, std::string b)
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
		cout << line << endl;
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
