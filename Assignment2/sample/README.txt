Database format:
1. Each line follows the order of:
	University -> Program -> Term -> Nationality -> Scholarship -> Enrolment, split by Tab.
2. In the dimension of University, we have 4 keys (index in code):
	UofA(0), UniSA(1), Flinders(2), Torrens(3).
3. In the dimension of Program, we have 6 keys (index in code):
	PhD(0), MCS(1), MSE(2), MCI(3), BCS(4), BSE(5).
4. In the dimension of Term, we have 5 keys (index in code):
	s1/2016(0), s2/2016(1), s1/2017(2), s2/2017(3), s1/2018(4).
5. In the dimension of Nationality, we have 6 keys (index in code):
	Australia(0), Korea(1), China(2), India(3), Iran(4), SriLanka(5).
6. In the dimension of Scholarship, we have 2 keys (index in code):
	No(0), Yes(1).
6. In the dimension of Enrolment, all the values are positive integers.

Query format:
1. Each line follows the order of:
	Query Type -> University -> Program -> Term -> Nationality -> Scholarship, split by Tab.
2. We have two types of query: SUM, TOP_k, where k is a positive integer.
3. The item started with "?" is the target of the TOP_k query where we only have one item started with "?".

Output format:
1. For the SUM query, if there is no matched record in the database, just cout a 0
2. For the TOP_k query:
	i. if no matched record in the database, cout NULL;
	ii. if the number of matched records less than k, cout all the matched records in decreasing order of enrolments;
	iii. if several matched records have the same enrolments, follows the order of the feature's index order, then cout the first k records.
3. The answer to each query takes one line.