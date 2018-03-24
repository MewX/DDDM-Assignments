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
#include <set>
#include <sstream>

using namespace std;

// type defines
typedef string ColumnName;
typedef map<ColumnName, string> Record;
typedef vector<string> ColumnValues;
typedef map<ColumnName, ColumnValues> Column;

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
	void makeAntiVersion()
	{
		op = ANTI_OP_MAP.at(op);
	}

	/**
	 * test whether the table record satisfies current predicate.
	 * @return true if satisfied; otherwise false
	 */
	bool satisfy(Record record)
	{
		// TODO
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
	vector<Column> db; // db<column names, column values>

public:
	/**
	 * default constructor
	 */
	Table() {}

	/**
	 * input is the keys' list.
	 * simply using the read first line, and split with '\t'
	 */
	Table(vector<ColumnName> prototypes) : Table()
	{
		//TODO: to upper case
	}

	/**
	 * get the number of rows of the Table
	 * @return the size
	 */
	int size()
	{
		// TODO:
	}

	/**
	 * get a record as key-value pair
	 * @return the <key, value> pair
	 */
	Record get(int i)
	{
		// TODO:
	}

	/**
	 * add a new record, note every field should be existing!
	 * @return false if failed; otherwise true
	 */
	bool add(Record data)
	{
		// TODO
	}

	/**
	 * delete a record by index
	 * @return false if failed; otherwise true
	 */
	bool del(int i)
	{
		// TODO:

	}

};

/**
 * a Query is just a set of valid predicates
 */
typedef set<Predicate> Query;
typedef vector<Query> Queries;

/**
 * predicates of the same attribute should be grouped together for its potential helps
 */
typedef set<Predicate> PredicateGroup; // grouped by attributes
typedef map<string, PredicateGroup> PredicateGroups; // <group attribute, predicates>

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
	PrimaryHorizentalFragmentation(const Table &db, const Queries queries, const PredicateGroups &predicates)
		: db(db), queries(queries), predicates(predicates) { }

	void run()
	{
		// TODO: the main runner
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
Table parseTable(vector<string> lines)
{
	Table table;
	vector<ColumnName> columnNames;
	for (int i = 0; i < lines.size(); i ++)
	{
		auto tokens = split(lines[i], '\t');
		if (i == 0)
		{
			// the fisrt record: attributes
			columnNames = tokens;
			table = Table(columnNames);
		}
		else
		{
			// the rest records
			Record record;
			for (int j = 0; j < columnNames.size(); j ++)
			{
				record.insert(columnNames[j], tokens[j]);
			}
			table.add(record);
		}
	}
	return table;
}

/**
 * only the non-empty line strings should be passed through here
 */
PredicateGroups parsePredicateGroups(vector<string> lines)
{
	// TODO:
}

/**
 * only the non-empty line strings should be passed through here
 */
Queries parseQueries(vector<string> queries)
{
	// TODO:
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
	ifstream fileDatabase(FILE_DATABASE, ios::in);
	while (fileDatabase >> line)
	{
		if (line.empty()) continue;
		lines.push_back(line);
	}
	fileDatabase.close();
	auto inTable = parseTable(lines);

	// read applications/queries
	lines.clear();
	ifstream fileApp(FILE_QUERY, ios::in);
	while (fileApp >> line)
	{
		if (line.empty()) continue;
		lines.push_back(line);
	}
	fileApp.close();
	auto inQueries = parseQueries(lines);

	// read predicates
	lines.clear();
	ifstream filePredicates(FILE_SIMPLE_PREDICATE, ios::in);
	while (filePredicates >> line)
	{
		if (line.empty()) continue;
		lines.push_back(line);
	}
	filePredicates.close();
	auto inPredicateGroups = parsePredicateGroups(lines);

	// run the algorithm
	PrimaryHorizentalFragmentation phf(inTable, inQueries, inPredicateGroups);
	phf.run();

	// used for Visual Studio 2017 debugging output
#ifdef _MSC_VER
	system("pause");
#endif
	return 0;
}
