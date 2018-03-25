/**
 * This file is the DDDM assignment 1 - PHF.cpp
 *
 * @author Yuanzhong Xia (a1700831)
 */
#include <fstream>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cassert>

using namespace std;

// type defines
typedef string ColumnName;
typedef map<ColumnName, string> Record;
typedef vector<string> ColumnValues;

/**
 * Supported operators
 */
enum Operator
{
	EQUAL,
	NOT_EQUAL,
	GREATER_THAN,
	LESS_THAN,
	GREATER_THAN_OR_EQUAL_TO,
	LESS_THAN_OR_EQUAL_TO,
};

/**
 * Global operator mapper
 */
const string OP_NOT_EQUAL = "!=";
const string OP_EQUAL = "="; // after NOT_EQUAL
const string OP_GREATER_THAN_OR_EQUAL_TO = ">=";
const string OP_GREATER_THAN = ">"; // after >=
const string OP_LESS_THAN_OR_EQUAL_TO = "<=";
const string OP_LESS_THAN = "<"; // after <=
const vector<pair<string, Operator>> OP_STRING_MAPPER =
{
	// the order MATTERS here
	{OP_NOT_EQUAL, NOT_EQUAL},
	{OP_GREATER_THAN_OR_EQUAL_TO, GREATER_THAN_OR_EQUAL_TO},
	{OP_LESS_THAN_OR_EQUAL_TO, LESS_THAN_OR_EQUAL_TO},
	{OP_EQUAL, EQUAL},
	{OP_GREATER_THAN, GREATER_THAN},
	{OP_LESS_THAN, LESS_THAN}
};

/**
 * The predicate is: key <op> val
 * e.g. TUITIONFEE <GREATER_THAN_OR_EQUAL_TO> 20
 */
class Predicate
{
private:
	// perform 'not' on op
	const map<Operator, Operator> ANTI_OP_MAP = {
		{EQUAL, NOT_EQUAL},
		{NOT_EQUAL, EQUAL},
		{GREATER_THAN, LESS_THAN_OR_EQUAL_TO},
		{LESS_THAN_OR_EQUAL_TO, GREATER_THAN},
		{LESS_THAN, GREATER_THAN_OR_EQUAL_TO},
		{GREATER_THAN_OR_EQUAL_TO, LESS_THAN}
	};

public:
	string key, val;
	Operator op;

	Predicate(string key, Operator op, string val)
	{
		this->key = key;
		this->op = op;
		this->val = val;
	}

	/**
	* perform the 'not' version of current <predicate>
	*/
	void makeAnti()
	{
		op = ANTI_OP_MAP.at(op);
	}

	/**
	 * test whether the table record satisfies current predicate.
	 * @return true if satisfied; otherwise false
	 */
	bool satisfy(Record record) const
	{
		const auto rVal = record.at(key);
		switch (op)
		{
		case EQUAL:
			return rVal == val;

		case NOT_EQUAL:
			return rVal != val;

		case GREATER_THAN:
			return atoi(rVal.c_str()) > atoi(val.c_str());

		case GREATER_THAN_OR_EQUAL_TO:
			return atoi(rVal.c_str()) >= atoi(val.c_str());

		case LESS_THAN:
			return atoi(rVal.c_str()) < atoi(val.c_str());

		case LESS_THAN_OR_EQUAL_TO:
			return atoi(rVal.c_str()) <= atoi(val.c_str());
		}
		return false;
	}
};

/**
 * The database table, storing them as columns because this structure might be used in future assignments.
 * This way makes it more extensible.
 */
class Table
{
private:
	// The index is currently the "vector<string>"'s index, may add primary key setting
	map<ColumnName, ColumnValues> db; // db<column names, column values>

public:
	/**
	 * default constructor
	 */
	Table() = default;

	/**
	 * input is the keys' list.
	 * simply using the read first line, and split with '\t'
	 */
	Table(vector<ColumnName> prototype) : Table()
	{
		for (auto &str : prototype)
		{
			transform(str.begin(), str.end(), str.begin(), ::toupper);
			db.insert({ str, ColumnValues() });
		}
	}

	/**
	 * get prototypes based on the order it was defined
	 */
	vector<ColumnName> getPrototype() const
	{
		vector<ColumnName> ret;
		for (const auto &c : db)
		{
			ret.push_back(c.first);
		}
		return ret;
	}

	/**
	 * get the number of rows of the Table
	 * @return the size
	 */
	int size() const
	{
		for (const auto &c : db)
		{
			return c.second.size();
		}
		return 0;
	}

	/**
	 * get a record as key-value pair
	 * @return the <key, value> pair
	 */
	Record get(int i) const
	{
		// this is an exception!
		assert(i >= 0 && i < size());

		Record ret;
		for (const auto &c : db)
		{
			ret.insert({ c.first, c.second[i] });
		}
		return ret;
	}

	/**
	 * add a new record, note every field should be existing!
	 * @return false if failed; otherwise true
	 */
	bool add(Record data)
	{
		// validate
		if (data.size() != db.size()) return false;

		for (const auto &d : data)
		{
			db.at(d.first).push_back(d.second);
		}
		return true;
	}

	/**
	 * delete a record by index
	 * @return false if failed; otherwise true
	 */
	bool del(int i)
	{
		if (i < 0 || i >= size()) return false;

		for (auto &c : db)
		{
			c.second.erase(c.second.begin() + i);
		}
		return true;
	}

};

/**
 * a Query is just a set of valid predicates
 */
typedef vector<Predicate> Query;
typedef vector<Query> Queries;

/**
 * predicates of the same attribute should be grouped together for its potential helps
 */
typedef vector<Predicate> PredicateGroup; // grouped by attributes
typedef map<string, PredicateGroup> PredicateGroups; // <group attribute, predicates>

/**
 * The fragment class on real nubmer domain
 */
class Fragment
{
private:
	vector<PredicateGroup> fragments;

public:
	Fragment() = default;

	/**
	 * the constructor takes all the predicates (can with different attributes)
	 */
	Fragment(const PredicateGroup &pg) : Fragment()
	{
		addPredicates(pg);
	}

	/**
	 * the basic operation for adding one new predicate and affecting the fragmentation
	 */
	void addSimplePredicate(Predicate p)
	{
		// TODO: the main logic for fragmenting
	}

	/**
	 * a set of predicates and they are fed for the fragmentation one by one
	 */
	void addPredicates(const PredicateGroup &pg)
	{
		for (const Predicate &p : pg)
		{
			addSimplePredicate(p);
		}
	}

	/**
	 * get a list of numbers (indexes) of the affected fragments by the query
	 */
	vector<int> getAffectedFragments(Query query)
	{
		// TODO:
	}

	const vector<PredicateGroup> &getAllFragments() const
	{
		return fragments;
	}
};

/**
 * This core class for PHF.
 */
class PrimaryHorizentalFragmentation
{
private:
	const Table &db;
	const Queries &queries;
	const PredicateGroups &predicates;

public:
	/**
	 * the constructor
	 */
	PrimaryHorizentalFragmentation(const Table &db, const Queries &queries, const PredicateGroups &predicates)
		: db(db), queries(queries), predicates(predicates) { }

	/**
	 * Validate rule 1: each fragment is accessed differently by at least one application
	 * i.e. no fragment is accessed by the same application set
	 */
	bool validateRule1(const PredicateGroup &pg)
	{
		// TODO: do the fragments
		Fragment f(pg);

	}

	/**
	 * validate relevant rule:
	 * acc(mi) / card(fi) != acc(mj) / card(fj)
	 */
	bool validateRelevant(const Predicate &p)
	{
		// TODO:
	}

	/**
	 * Validate whether current Pr' is completel
	 */
	bool validateComplete(const PredicateGroup &PrQuote)
	{
		// TODO
	}

	/**
	 * the COM_MIN algorithm
	 */
	PredicateGroup comMin()
	{
		// map to one-dimension array for this function use only
		PredicateGroup mergedPredicateGroup;
		for (const auto &pg : predicates)
		{
			for (const auto &p : pg.second)
			{
				mergedPredicateGroup.push_back(p);
			}
		}

		// begin of COM_MIN algorithm
		const auto &R = db;
		auto &Pr = mergedPredicateGroup;
		PredicateGroup PrQuote; // the output

		for (unsigned i = 0; i < Pr.size(); i++)
		{
			PrQuote.push_back(Pr[i]); // Pr' = pi
			if (validateRule1(PrQuote))
			{
				// valid and use it
				Pr.erase(Pr.begin() + i); // Pr = Pr - pi
				break;
			}
			PrQuote.pop_back();
		}

		// main loop
		do
		{
			// find a pj in Pr, and pj partitions some current fragments according to Rule 1
			for (unsigned i = 0; i < Pr.size(); i++)
			{
				PrQuote.push_back(Pr[i]); // Pr' = Pr' UNION pi
				if (validateRule1(PrQuote))
				{
					// valid and use it
					Pr.erase(Pr.begin() + i); // Pr = Pr - pi
					i--; // the size gets dropped

					// if existing pk in Pr' which is not relevant
					for (unsigned k = 0; k < PrQuote.size(); k++)
					{
						if (!validateRelevant(PrQuote[k]))
						{
							PrQuote.erase(PrQuote.begin() + k); // Pr' = Pr' - pk
							k--; // the size gets dropped
						}
					}
				}
				else {
					PrQuote.pop_back();
				}
			}

		} while (validateComplete(PrQuote));

		return PrQuote;
	}

	/**
	 * determine the set M of minterm prediates
	 */
	vector<PredicateGroup> calcSetOfMintermPredicates()
	{
		// TODO
	}

	/**
	 * determin the set I of implications among pi in Pr'
	 */
	vector<PredicateGroup> calcImplicationsAmongPrQuote()
	{
		// TODO
	}

	/**
	 * test whether mi is contradictory according to I
	 */
	bool isContradictory(const vector<PredicateGroup> &I, const PredicateGroup &mi)
	{
		// TODO
	}

	/**
	 * the algorithm 3.2: PHORIZONTAL algorithm
	 */
	vector<PredicateGroup> pHorizontal()
	{
		auto PrQuote = comMin();
		auto M = calcSetOfMintermPredicates();
		auto I = calcImplicationsAmongPrQuote();

		for (unsigned i = 0; i < M.size(); i++)
		{
			if (isContradictory(I, M[i]))
			{
				M.erase(M.begin() + i);
				i--;
			}
		}
		return M;
	}

	/**
	 * based on the description of PHORIZONTAL algorithm, it doesn't remove the empty fragments
	 * so, here I remove all the redundent predicate group
	 */
	vector<PredicateGroup> clearEmptyFragments(vector<PredicateGroup> predicateGroups)
	{
		// TODO
	}

	/**
	 * print all the results
	 */
	void printResult()
	{
		// TODO
	}

	void run()
	{
		auto result = pHorizontal();

		// remove empty sets (this function is not part of PHORIZONTAL algorithm)
		result = clearEmptyFragments(result);

		// sort and output
		printResult();
	}

};

/**
 * The split function for C++ string.
 */
vector<string> split(const std::string &s, const char delim)
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
 * only the non-empty line strings should be passed through here
 */
Table parseTable(const vector<string> &lines)
{
	Table table;
	vector<ColumnName> columnNames;
	for (unsigned i = 0; i < lines.size(); i ++)
	{
		auto tokens = split(lines[i], '\t');
		if (i == 0)
		{
			// the fisrt record: attributes
			columnNames = tokens;

			// convert to upper cases
			for (auto &str : columnNames)
			{
				transform(str.begin(), str.end(), str.begin(), ::toupper);
			}

			// create new table with thoe columns
			table = Table(columnNames);
		}
		else
		{
			// the rest records
			Record record;
			for (unsigned j = 0; j < columnNames.size(); j ++)
			{
				record[columnNames[j]] = tokens[j];
			}
			table.add(record);
		}
	}
	return table;
}

/**
 * only the non-empty line strings should be passed through here
 */
PredicateGroups parsePredicateGroups(const vector<string> &lines)
{
	PredicateGroups pgs;
	for (const auto &line : lines)
	{
		PredicateGroup predicates;
		string attr; // the attribute name used for the map

		auto tokens = split(line, '\t');
		string keyTemp;
		for (const auto &token : tokens)
		{
			for (const auto &opMapper : OP_STRING_MAPPER)
			{
				const auto idx = token.find(opMapper.first);
				if (idx != string::npos)
				{
					const Predicate p = Predicate(token.substr(0, idx), opMapper.second, token.substr(idx + opMapper.first.length()));
					predicates.push_back(p);

					// set key for the attribute map
					if (keyTemp.empty()) keyTemp = p.key;
					break;
				}
			}
		}
		pgs.insert({ {keyTemp, predicates} });
	}
	return pgs;
}

/**
 * only the non-empty line strings should be passed through here
 */
Queries parseQueries(const vector<string> &lines, const vector<ColumnName> &columnNames)
{
	Queries queries;
	for (const auto &line : lines)
	{
		Query q;

		auto tokens = split(line, '\t');
		for (unsigned i = 0; i < tokens.size(); i ++)
		{
			const auto &token = tokens[i];
			const auto idxDash = token.find('-');
			if (idxDash == string::npos || idxDash == 0)
			{
				// instant value (instant negative is also considered)
				// TODO: the first column is primary key, may add PK support later
				q.push_back(Predicate(columnNames[i + 1], EQUAL, token));
			}
			else
			{
				// <KEY>-<OP><VALUE>
				const string key = token.substr(0, idxDash); // key can be extracted here
				const string theRest = token.substr(idxDash + 1); // skip the dash itself
				if (theRest == "?")
				{
					q.push_back(Predicate(key, EQUAL, theRest));
				}
				else if (theRest != "ALL")
				{
					// "ALL" means this attribute is ignored
					for (const auto &opMapper : OP_STRING_MAPPER)
					{
						// find op and val
						const auto idx = theRest.find(opMapper.first);
						if (idx != string::npos)
						{
							const string val = theRest.substr(idx + opMapper.first.length());
							const Predicate p = Predicate(key, opMapper.second, val);
							q.push_back(p);
							break;
						}
					}
				}
			}
		}
		queries.push_back(q);
	}

	return queries;
}

/**
 * This main function
 */
int main(int argc, char **argv)
{
	const string FILE_DATABASE = argv[1];
	const string FILE_QUERY = argv[2];
	const string FILE_SIMPLE_PREDICATE = argv[3];

	string line;
	vector<string> lines;

	// read database file
	ifstream fileDatabase(FILE_DATABASE);
	while (getline(fileDatabase, line))
	{
		if (line.empty()) continue;
		lines.push_back(line);
	}
	fileDatabase.close();
	const auto inTable = parseTable(lines);

	// read applications/queries
	lines.clear();
	ifstream fileApp(FILE_QUERY);
	while (getline(fileApp, line))
	{
		if (line.empty()) continue;
		lines.push_back(line);
	}
	fileApp.close();
	const auto inQueries = parseQueries(lines, inTable.getPrototype());

	// read predicates
	lines.clear();
	ifstream filePredicates(FILE_SIMPLE_PREDICATE);
	while (getline(filePredicates, line))
	{
		if (line.empty()) continue;
		lines.push_back(line);
	}
	filePredicates.close();
	const auto inPredicateGroups = parsePredicateGroups(lines);

	// run the algorithm
	PrimaryHorizentalFragmentation phf(inTable, inQueries, inPredicateGroups);
	phf.run();

	// used for Visual Studio 2017 debugging output
#ifdef _MSC_VER
	system("pause");
#endif
	return 0;
}
