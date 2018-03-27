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
typedef map<ColumnName, string> Record; // Record = Tuple
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

// perform 'not' on op
const map<Operator, Operator> ANTI_OP_MAP = {
	{EQUAL, NOT_EQUAL},
	{NOT_EQUAL, EQUAL},
	{GREATER_THAN, LESS_THAN_OR_EQUAL_TO},
	{LESS_THAN_OR_EQUAL_TO, GREATER_THAN},
	{LESS_THAN, GREATER_THAN_OR_EQUAL_TO},
	{GREATER_THAN_OR_EQUAL_TO, LESS_THAN}
};

/**
 * The predicate is: key <op> val
 * e.g. TUITIONFEE <GREATER_THAN_OR_EQUAL_TO> 20
 */
class Predicate
{
public:
	string key, val;
	Operator op;

	Predicate() : Predicate("", EQUAL, "0") { }

	Predicate(const string &key, const Operator op, const string &val)
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
	bool satisfy(const Record &record) const
	{
		const auto rVal = record.at(key);
		switch (op)
		{
		case EQUAL:
			return rVal == val || val == "?"; // Note: query target is not considered in assignment 1

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

	/**
	 * overload the equal function
	 */
	bool operator== (const Predicate &b) const
	{
		return val == b.val && op == b.op && key == b.key;
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
	unsigned size() const
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
	Record get(unsigned i) const
	{
		// this is an exception!
		assert(i < size());

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
	bool del(unsigned i)
	{
		if (i >= size()) return false;

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
	 * test whether given p can fragment current fragments according to Rule1
	 * TODO: this is not working with rule 1 currently
	 */
	bool canPartitionCurrent(const Predicate &p) const
	{
		// TODO: rule 1
		if (fragments.empty()) return true;

		// for each fragment
		for (int i = fragments.size() - 1; i >= 0; i --)
		{
			PredicateGroup relevantPredicates = getPredicatesByAttr(i, p.key);
			if (relevantPredicates.empty()) return true; // no, need to create for this attribute

			// not empty, need to know whether it is [=/!=] or [</<=/>=/>]
			// one of them is enough for knowing the type
			if (p.op == EQUAL || p.op == NOT_EQUAL)
			{
				assert(relevantPredicates[0].op == NOT_EQUAL || relevantPredicates.size() == 1);

				// [=/!=]
				if (relevantPredicates[0].op == NOT_EQUAL)
				{
					// only one case that can generate a new fragment
					bool existing = false;
					for (const auto &temp : relevantPredicates)
					{
						if (temp.val == p.val)
						{
							existing = true;
							break;
						}
					}

					// not existing 
					if (!existing) return true;
				} // if it's EQUAL, nothing to do
			}
			else
			{
				// [</<=/>=/>]
				if (relevantPredicates.size() == 1)
				{
					// the simple case
					const Predicate tempPredicate = relevantPredicates[0];
					if (tempPredicate.op == GREATER_THAN || tempPredicate.op == GREATER_THAN_OR_EQUAL_TO)
					{
						Predicate temp = p;
						if (temp.op == LESS_THAN || temp.op == LESS_THAN_OR_EQUAL_TO) temp.makeAnti(); // convert to anti form

						if (temp == tempPredicate) continue; // same predicate, no need to continue
						assert(temp.op == GREATER_THAN || temp.op == GREATER_THAN_OR_EQUAL_TO);

						// too complicated case, not consider right now
						// e.g. > 10 & >= 10 --> > 10 & = 10
						assert(tempPredicate.val != temp.val);

						if (tempPredicate.val < temp.val)
						{
							return true;
						} // tempPredicate.val > p.val, not splitable
					}
					else
					{
						// must be </<=
						Predicate temp = p;
						if (temp.op == GREATER_THAN || temp.op == GREATER_THAN_OR_EQUAL_TO) temp.makeAnti(); // convert to anti form

						if (temp == tempPredicate) continue; // same predicate, no need to continue
						assert(temp.op == LESS_THAN || temp.op == LESS_THAN_OR_EQUAL_TO);

						// too complicated case, not consider right now
						// e.g. < 10 & <= 10 --> < 10 & = 10
						assert(tempPredicate.val != temp.val);

						if (tempPredicate.val > temp.val)
						{
							return true;
						} // tempPredicate.val < p.val, not splitable
					}
				}
				else
				{
					// can only be size 2
					assert(relevantPredicates.size() == 2);
					// between case, sort from op = >/>= to op = </<=
					if (relevantPredicates[0].op == LESS_THAN || relevantPredicates[0].op == LESS_THAN_OR_EQUAL_TO)
					{
						swap(relevantPredicates[0], relevantPredicates[1]);
					}

					assert(relevantPredicates[0].op == GREATER_THAN || relevantPredicates[0].op == GREATER_THAN_OR_EQUAL_TO);
					assert(relevantPredicates[1].op == LESS_THAN || relevantPredicates[1].op == LESS_THAN_OR_EQUAL_TO);

					// ignore this case
					if (p.val <= relevantPredicates[0].val || p.val >= relevantPredicates[1].val) continue;
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * get the predication group from fragment[index] where the attribute is <key>
	 */
	PredicateGroup getPredicatesByAttr(const unsigned index, const string &key) const
	{
		assert(index < fragments.size());

		PredicateGroup pg;
		for (const Predicate &p : fragments[index])
		{
			if (p.key == key)
			{
				pg.push_back(p);
			}
		}
		return pg;
	}

	/**
	 * get the prediction group from fragment[index] where the sttribute of <key> is eliminated
	 */
	PredicateGroup getPredicatesWithSpecifiedAttrCleared(const unsigned index, const string &key) const
	{
		assert(index < fragments.size());

		PredicateGroup pg;
		for (const Predicate &p : fragments[index])
		{
			if (p.key != key)
			{
				pg.push_back(p);
			}
		}
		return pg;
	}

	/**
	 * the basic operation for adding one new predicate and affecting the fragmentation
	 */
	void addSimplePredicate(const Predicate &p)
	{
		Predicate anti = p;
		anti.makeAnti();

		// if there's no fragment
		if (fragments.empty())
		{
			fragments.push_back({ Predicate(p.key, p.op, p.val) });
			fragments.push_back({ Predicate(anti.key, anti.op, anti.val) });
			return;
		}

		// for each fragment
		for (int i = fragments.size() - 1; i >= 0; i --)
		{
			const PredicateGroup &fragment = fragments[i];
			PredicateGroup relevantPredicates = getPredicatesByAttr(i, p.key);
			if (relevantPredicates.empty())
			{
				// no, need to create for this attribute
				fragments.push_back({ Predicate(p.key, p.op, p.val) });
				fragments.push_back({ Predicate(anti.key, anti.op, anti.val) });
				break;
			}

			// not empty, need to know whether it is [=/!=] or [</<=/>=/>]
			// one of them is enough for knowing the type
			if (p.op == EQUAL || p.op == NOT_EQUAL)
			{
				assert(relevantPredicates[0].op == NOT_EQUAL || relevantPredicates.size() == 1);

				// [=/!=]
				if (relevantPredicates[0].op == NOT_EQUAL)
				{
					// only one case that can generate a new fragment
					bool existing = false;
					for (const auto &temp : relevantPredicates)
					{
						if (temp.val == p.val)
						{
							existing = true;
							break;
						}
					}

					// not existing 
					if (!existing)
					{
						Predicate pPositive = p;
						if (pPositive.op == NOT_EQUAL) pPositive.makeAnti();
						assert(p.op == EQUAL);
						Predicate pNegative = pPositive;
						pNegative.makeAnti();
						assert(pNegative.op == NOT_EQUAL);

						PredicateGroup manyNotEqual = fragment; // current negative case
						manyNotEqual.push_back(pNegative); // one more negative
						PredicateGroup oneEqual = getPredicatesWithSpecifiedAttrCleared(i, p.key);
						oneEqual.push_back(pPositive); // one positive case
						
						// update fragments
						fragments.erase(fragments.begin() + i);
						fragments.push_back(manyNotEqual);
						fragments.push_back(oneEqual);
					}
				} // if it's EQUAL, nothing to do
			}
			else
			{
				// [</<=/>=/>]
				if (relevantPredicates.size() == 1)
				{
					// the simple case
					const Predicate tempPredicate = relevantPredicates[0];
					if (tempPredicate.op == GREATER_THAN || tempPredicate.op == GREATER_THAN_OR_EQUAL_TO)
					{
						Predicate temp = p;
						if (temp.op == LESS_THAN || temp.op == LESS_THAN_OR_EQUAL_TO) temp.makeAnti(); // convert to anti form

						if (temp == tempPredicate) continue; // same predicate, no need to continue
						assert(temp.op == GREATER_THAN || temp.op == GREATER_THAN_OR_EQUAL_TO);

						// too complicated case, not consider right now
						// e.g. > 10 & >= 10 --> > 10 & = 10
						assert(tempPredicate.val != temp.val);

						if (tempPredicate.val < temp.val)
						{
							// split into two
							Predicate leftRight = temp; // </<= tempPredicate </<= leftRight/temp
							leftRight.makeAnti();
							assert(leftRight.op == LESS_THAN || leftRight.op == LESS_THAN_OR_EQUAL_TO);

							PredicateGroup leftPart = getPredicatesWithSpecifiedAttrCleared(i, p.key);
							PredicateGroup rightPart = leftPart; // memory copy is a bit faster
							leftPart.push_back(tempPredicate);
							leftPart.push_back(leftRight);
							rightPart.push_back(temp);

							// update fragments
							fragments.erase(fragments.begin() + i);
							fragments.push_back(leftPart);
							fragments.push_back(rightPart);
						} // tempPredicate.val > p.val, not splitable
					}
					else
					{
						// must be </<=
						Predicate temp = p;
						if (temp.op == GREATER_THAN || temp.op == GREATER_THAN_OR_EQUAL_TO) temp.makeAnti(); // convert to anti form

						if (temp == tempPredicate) continue; // same predicate, no need to continue
						assert(temp.op == LESS_THAN || temp.op == LESS_THAN_OR_EQUAL_TO);

						// too complicated case, not consider right now
						// e.g. < 10 & <= 10 --> < 10 & = 10
						assert(tempPredicate.val != temp.val);

						if (tempPredicate.val > temp.val)
						{
							// split into two
							Predicate rightLeft = temp; // temp/rightLeft </<= tempPredicate </<= 
							rightLeft.makeAnti();
							assert(rightLeft.op == GREATER_THAN || rightLeft.op == GREATER_THAN_OR_EQUAL_TO);

							PredicateGroup leftPart = getPredicatesWithSpecifiedAttrCleared(i, p.key);
							PredicateGroup rightPart = leftPart; // memory copy is a bit faster
							rightPart.push_back(tempPredicate);
							rightPart.push_back(rightLeft);
							leftPart.push_back(temp);

							// update fragments
							fragments.erase(fragments.begin() + i);
							fragments.push_back(leftPart);
							fragments.push_back(rightPart);
						} // tempPredicate.val < p.val, not splitable
					}
				}
				else
				{
					// can only be size 2
					// between case, sort from op = >/>= to op = </<=
					if (relevantPredicates[0].op == LESS_THAN || relevantPredicates[0].op == LESS_THAN_OR_EQUAL_TO)
					{
						swap(relevantPredicates[0], relevantPredicates[1]);
					}

					assert(relevantPredicates[0].op == GREATER_THAN || relevantPredicates[0].op == GREATER_THAN_OR_EQUAL_TO);
					assert(relevantPredicates[1].op == LESS_THAN || relevantPredicates[1].op == LESS_THAN_OR_EQUAL_TO);

					// ignore this case
					if (p.val <= relevantPredicates[0].val || p.val >= relevantPredicates[1].val) continue;

					// now p.val is within the section, find intersection
					// convert >/>= into </<=
					Predicate temp = p;
					if (temp.op == GREATER_THAN || temp.op == GREATER_THAN_OR_EQUAL_TO) temp.makeAnti();

					// now temp.op is </<=
					Predicate forRight = temp; // temp if for left
					forRight.makeAnti();
					assert(forRight.op == GREATER_THAN || forRight.op == GREATER_THAN_OR_EQUAL_TO);

					PredicateGroup leftPart = getPredicatesWithSpecifiedAttrCleared(i, p.key);
					PredicateGroup rightPart = leftPart; // memory copy is a bit faster
					leftPart.push_back(relevantPredicates[0]);
					leftPart.push_back(temp);
					rightPart.push_back(relevantPredicates[1]);
					rightPart.push_back(forRight);

					// update fragments
					fragments.erase(fragments.begin() + i);
					fragments.push_back(leftPart);
					fragments.push_back(rightPart);
				}
			}
		}
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

	const vector<PredicateGroup> &getAllFragments() const
	{
		return fragments;
	}
};

/**
 * For reuse of fragment statistics
 */
typedef map<unsigned, vector<unsigned>> FragmentDetail;
typedef map<unsigned, int> RecordDetail;

/**
 * This core class for PHF.
 */
class PrimaryHorizentalFragmentation
{
private:
	const Table &db;
	const Queries &queries;
	const PredicateGroups &predicates;

	/**
	 * Make statistics on given minterm predicates
	 * 1. each fragment - contained records
	 * 2. each record - its access frequency
	 */
	pair<FragmentDetail, RecordDetail> doStatistics(const PredicateGroup &PrQuote) const
	{
		const Fragment f(PrQuote);
		const auto &fragments = f.getAllFragments();

		FragmentDetail fragmentDetail; // <fragment id, records>
		RecordDetail recordDetail; // <record id, access frequency>
		
		// q is query index
		for (const auto &q : queries)
		{
			// i is record index
			for (unsigned i = 0; i < db.size(); i++)
			{
				// if it't selected from this query
				bool satisfyAll = true;
				for (const auto &pred : q)
				{
					if (!pred.satisfy(db.get(i)))
					{
						satisfyAll = false;
						break;;
					}
				}
				if (!satisfyAll) continue; // curent record is not selected

				// j is fragment index
				for (unsigned j = 0; j < fragments.size(); j++)
				{
					const auto &fragment = fragments[j];
					satisfyAll = true;
					for (const auto &p : fragment)
					{
						if (!p.satisfy(db.get(i)))
						{
							satisfyAll = false;
							break;
						}
					}

					// available record
					if (satisfyAll)
					{
						if (recordDetail.count(i) == 0) recordDetail[i] = 1;
						else recordDetail[i] = recordDetail[i] + 1;

						if (fragmentDetail.count(j) == 0) fragmentDetail.insert({ j, { i } });
						else fragmentDetail.at(j).push_back(i);
					}
				}
			}
		}

		return { fragmentDetail, recordDetail };
	}

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
	bool validateRule1(const PredicateGroup &pg) const
	{
		// TODO: do the fragments
		Fragment f(pg);

		return false;
	}

	/**
	 * validate relevant rule:
	 * acc(mi) / card(fi) != acc(mj) / card(fj)
	 */
	bool validateRelevant(const PredicateGroup &pg, const Predicate &p) const
	{
		PredicateGroup copy = pg;
		for (unsigned i = 0; i < copy.size(); i++)
		{
			// copy current minterm predicates where the selected p was ignored
			if (!(copy[i] == p))
			{
				copy.erase(copy.begin() + i);
				break;
			}
		}

		// do the statistics
		const auto &fragmentDetail = doStatistics(copy).first; // fragment id - list of record ids

		// loop through all the applications
		// acc(mi) == acc(qi)
		int accNm = 0, accNot = 0;
		for (const auto &app : queries)
		{
			for (const auto &predicate : app)
			{
				auto tp = p;
				if (predicate == tp) accNm++;

				tp.makeAnti();
				if (predicate == tp) accNot++;
			}
		}

		// it's not used by either application
		if (accNm == 0 && accNot == 0) return false;

		// each fragmeent
		for (const auto &finfo : fragmentDetail)
		{
			// normal form, 'not' form
			int cardNm = 0, cardNot = 0;

			const auto &fid = finfo.first; // fragment id
			const auto &records = finfo.second; // fragment records

			// see the two cards
			for (const auto &recordId : records)
			{
				// for each record, test whether it's satisfy p
				auto tp = p;
				if (tp.satisfy(db.get(recordId))) cardNm++;
				else cardNot++;
			}

			// I don't want useless frgments
			if (cardNm == 0 || cardNot == 0) continue;

			// calc relevant
			if (abs(1.0 * accNm / cardNm - 1.0 * accNot / cardNot) < 0.0001) return true;
		}

		return false;
	}

	/**
	 * Validate whether current Pr' is completel
	 */
	bool validateComplete(const PredicateGroup &PrQuote) const
	{
		const auto temp = doStatistics(PrQuote);
		const auto &fragmentDetail = temp.first;
		const auto &recordDetail = temp.second;

		// validate: in every fragment, the records inside should have the same access frequency
		map<int, int> fragmentValidator; // <fragment id, access frenquency that should be>
		for (const auto &finfo : fragmentDetail)
		{
			const int &fid = finfo.first;
			for (const auto &recordId : finfo.second)
			{
				if (fragmentValidator.count(fid) == 0) fragmentValidator[fid] = recordDetail.at(recordId);
				else if (fragmentValidator[fid] != recordDetail.at(recordId)) return false;
			}
		}

		return true;
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
						if (!validateRelevant(PrQuote, PrQuote[k]))
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

		} while (!validateComplete(PrQuote));

		return PrQuote;
	}

	/**
	 * determine the set M of minterm prediates
	 */
	vector<PredicateGroup> calcSetOfMintermPredicates()
	{
		// TODO
		return {};
	}

	/**
	 * determin the set I of implications among pi in Pr'
	 */
	vector<PredicateGroup> calcImplicationsAmongPrQuote()
	{
		// TODO
		return {};
	}

	/**
	 * test whether mi is contradictory according to I
	 */
	bool isContradictory(const vector<PredicateGroup> &I, const PredicateGroup &mi)
	{
		// TODO
		return false;
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
		return {};
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
