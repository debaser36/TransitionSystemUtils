#pragma warning(disable:4996)
#include <fstream>
#include <iostream>
#include "procSystem.h"
namespace
{
	[[maybe_unused]] LTS HA009_24OnlyQ()
	{
		LTS lts(30,10);
		lts.addTransition("q1", "q2", "c");
		lts.addTransition("q1", "q3", "a");
		lts.addTransition("q1", "q4", "c");

		lts.addTransition("q2", "q5", "c");

		lts.addTransition("q3", "q3", "a");
		lts.addTransition("q3", "q2", "c");
		lts.addTransition("q3", "q6", "c");

		lts.addTransition("q4", "q7", "a");
		lts.addTransition("q4", "q7", "c");

		lts.addTransition("q5", "q7", "b");

		lts.addTransition("q6", "q7", "a");
		lts.addTransition("q6", "q7", "c");

		lts.addTransition("q7", "q5", "c");

		return lts;
	}
	LTS HA009_24()
{
	LTS lts(30,10);
	lts.addTransition("p1", "p2", "c");
	lts.addTransition("p1", "p1", "a");
	lts.addTransition("p1", "p4", "c");
	lts.addTransition("p1", "p5", "a");
	lts.addTransition("p1", "p5", "c");
	lts.addTransition("p2", "p3", "c");
	lts.addTransition("p3", "p6", "b");
	lts.addTransition("p4", "p3", "c");
	lts.addTransition("p5", "p6", "a");
	lts.addTransition("p5", "p6", "c");
	lts.addTransition("p6", "p3", "c");

	lts.addTransition("q1", "q2", "c");
	lts.addTransition("q1", "q3", "a");
	lts.addTransition("q1", "q4", "c");
	lts.addTransition("q2", "q5", "c");
	lts.addTransition("q3", "q3", "a");
	lts.addTransition("q3", "q2", "c");
	lts.addTransition("q3", "q6", "c");
	lts.addTransition("q4", "q7", "a");
	lts.addTransition("q4", "q7", "c");
	lts.addTransition("q5", "q7", "b");
	lts.addTransition("q6", "q7", "a");
	lts.addTransition("q6", "q7", "c");
	lts.addTransition("q7", "q5", "c");

	lts.addTransition("r1", "r2", "a");
	lts.addTransition("r1", "r2", "c");

	lts.addTransition("r2", "r3", "c");

	lts.addTransition("r3", "r2", "b");

	lts.addTransition("r4", "r3", "c");

	lts.addTransition("r5", "r1", "c");
	lts.addTransition("r5", "r7", "a");


	lts.addTransition("r6", "r7", "a");
	lts.addTransition("r6", "r4", "c");

	lts.addTransition("r7", "r5", "a");
	lts.addTransition("r7", "r6", "a");
	lts.addTransition("r7", "r1", "c");
	lts.addTransition("r7", "r4", "c");
	lts.addTransition("r7", "r7", "a");

	return lts;
}

	[[maybe_unused]] LTS ha007TransitionSystem()  // NOLINT(misc-use-anonymous-namespace)
{
	LTS ts(40,40);
	ts.addTransition("p1", "p5", "c");

	ts.addTransition("p2", "p2", "a");
	ts.addTransition("p2", "p6", "a");

	ts.addTransition("p3", "p4", "a");
	ts.addTransition("p3", "p4", "b");
	ts.addTransition("p3", "p8", "b");

	ts.addTransition("p5", "p4", "c");
	ts.addTransition("p5", "p9", "b");

	ts.addTransition("p6", "p11", "a");
	ts.addTransition("p6", "p3", "d");

	ts.addTransition("p7", "p8", "c");
	ts.addTransition("p7", "p8", "b");

	ts.addTransition("p9", "p4", "a");

	ts.addTransition("p10", "p5", "c");
	ts.addTransition("p10", "p9", "b");

	ts.addTransition("p11", "p6", "a");
	ts.addTransition("p11", "p13", "d");


	ts.addTransition("p12", "p7", "c");

	ts.addTransition("p13", "p4", "a");
	ts.addTransition("p13", "p8", "b");

	ts.addTransition("p14", "p14", "a");
	ts.addTransition("p14", "p13", "d");

	return ts;
}
}


int main()
{
	LTS ltsHA009 = HA009_24();
	// LTS t2 = ha007TransitionSystem();

	const std::vector<ProcPair> bisim = getBisimilarity(ltsHA009);

	std::ofstream f;
	f.open("bisim.txt", std::ios::out | std::ios::trunc);
	printProcProcRelation(bisim, f, &ltsHA009);
	bool a = findInRelation("p1", "q3", bisim);
	bool b = findInRelation("q1", "r7", bisim);
	bool c = findInRelation("r5", "p1", bisim);
	bool d = findInRelation("q1", "q3", bisim);
	//bool test_e = findInRelation("p1", "p1", bisim);
	//bool test_f = findInRelation("p2", "r4", bisim);

	f.close();
	return 0;
}
