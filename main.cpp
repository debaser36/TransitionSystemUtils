#pragma warning(disable:4996)
#include <algorithm>
#include <iostream>
#include <unordered_set>


enum class e_side
{
	left = 0,
	right
};

/**
 * \brief Only the arrow function describing the transition as char and the process the arrow points to
 */
class Transition
{
public:
	int toProc;
	char tran;

	Transition()
	{
		this->toProc = 0;
		this->tran = 0;
	}
	Transition(int to, char symbol)
	{
		this->toProc = to;
		this->tran = symbol;
	}


	
	bool operator==(const Transition& lhs) const
	{
		return this->toProc == lhs.toProc && this->tran == lhs.tran;
	}
	bool operator!=(const Transition& rhs) const
	{
		return !(*this == rhs);
	}

};
namespace std
{
	template<>
	struct hash<Transition>
	{
		size_t operator()(const Transition& obj) const
		{
			std::size_t seed = 0x57B96258;
			seed ^= (seed << 6) + (seed >> 2) + 0x3B5682C4 + static_cast<std::size_t>(obj.toProc);
			seed ^= (seed << 6) + (seed >> 2) + 0x52F3423C + static_cast<std::size_t>(obj.tran);
			return seed;
		}
	};
}


/**
 * \brief Process with number and all transitions
 */
class Proc
{

public:
	int procNumber;
	std::unordered_set<Transition> transitions;


	Proc()
	{
		this->procNumber = -1;
		this->transitions = std::unordered_set<Transition>();
	}
	Proc(int procNumber)
	{
		this->procNumber = procNumber;
		this->transitions = std::unordered_set <Transition>();
	}
	void addTransition(int to, char tran)
	{
		Transition t(to, tran);
		transitions.emplace(t);
	}

	bool operator==(const Proc& rhs) const
	{
		return this->procNumber == rhs.procNumber;
	}

	bool operator!=(const Proc& rhs) const
	{
		return !(*this == rhs);
	}



	/**
	 * \brief 
	 * \param tranToFind char of the transition to find from the process
	 * \return the procID, when found, -1 otherwise
	 */
	int findTransition(const char tranToFind) const
	{
		for (auto& t : this->transitions)
		{
			if (t.tran == tranToFind) return t.toProc;
		}
		return -1;
	}

	/**
	 * \brief 
	 * \param tranToFind char of the transitions to find from the process
	 * \return unordered_set of all the procID's
	 */
	std::unordered_set<int> findAllTransitions(const char tranToFind) const
	{
		std::unordered_set<int> retMe;
		for(auto& t: this->transitions)
		{
			if (t.tran == tranToFind) retMe.emplace(t.toProc);
		}
		return retMe;
	}
};


/**
 * \brief is NOT equal under the symmetric completion --> (a,b) != (b, a)
 */
class procPair
{
public:
	int procOne;
	int procTwo;

	

	bool operator==(const procPair& rhs) const
	{
		return (this->procOne == rhs.procOne
			&& this->procTwo == rhs.procTwo);
	}


	bool operator!=(const procPair& rhs) const
	{
		return !(*this == rhs);
	}

	friend bool operator<(const procPair& lhs, const procPair& rhs)
	{
		if (lhs.procOne < rhs.procOne)
			return true;
		if (rhs.procOne < lhs.procOne)
			return false;
		return lhs.procTwo < rhs.procTwo;
	}

	friend bool operator<=(const procPair& lhs, const procPair& rhs)
	{
		return !(rhs < lhs);
	}

	friend bool operator>(const procPair& lhs, const procPair& rhs)
	{
		return rhs < lhs;
	}

	friend bool operator>=(const procPair& lhs, const procPair& rhs)
	{
		return !(lhs < rhs);
	}


	procPair(int procOne, int procTwo)
		: procOne(procOne),
		procTwo(procTwo)
	{
	}
};


namespace std
{
	template<>
	struct hash<Proc>
	{
		size_t operator()(const Proc& obj) const
		{
			std::size_t seed = 0x48D9E551;
			seed ^= (seed << 6) + (seed >> 2) + 0x31814156 + static_cast<std::size_t>(obj.procNumber);
			return seed;
		}
	};

	template<>
	struct hash<procPair>
	{
		size_t operator()(const procPair& obj) const
		{
			std::size_t seed = 0x7E6FA0B8;
			seed ^= (seed << 6) + (seed >> 2) + 0x4FB095BD + static_cast<std::size_t>(obj.procOne);
			seed ^= (seed << 6) + (seed >> 2) + 0x42802B3A + static_cast<std::size_t>(obj.procTwo);
			return seed;
		}
	};

}


class TranSystem
{

public:

	int procCount;
	std::unordered_set<Proc> processes;


	TranSystem()
	{
		this->procCount = -1;
		this->processes = std::unordered_set<Proc>();
	}

	explicit TranSystem(int procCount)
	{
		this->procCount = procCount;
		processes = std::unordered_set<Proc>();

		for (int i = 1; i <= procCount; i++)
		{
			Proc addMe(i);
			this->processes.emplace(addMe);
		}
	}
	void addTransition(const int from, const int to, const char tran) const
	{
		const auto p = this->findProcFromId(from);
		if (!p || from > procCount || to > procCount || from < 1 || to < 1) throw std::exception("ERROR, couldn't find process");
		const auto pNotConst = const_cast<Proc*>(p);
		pNotConst->addTransition(to, tran);
	}



	/**
	 * \brief
	 * \return (Proc x Proc)
	 */
	std::unordered_set<procPair> returnPROCxPROC() const
	{
		std::unordered_set<procPair> retMe;
		for(auto& proc1 : this->processes)
			for(auto& proc2: this->processes)
			{
				procPair pairToAdd(proc1.procNumber, proc2.procNumber);
				retMe.emplace(pairToAdd);
			}
		return retMe;
	}


	/**
	 * \brief returns the process in the LTS, if found
	 * \param procID procID to find
	 * \return Proc Element or NULLL
	 */
	const Proc * findProcFromId(int procID) const
	{
		for (auto& p : this->processes)
		{
			if (p.procNumber == procID) return &p;
		}
		return nullptr;
	}


	/**
	 * \brief F Function
	 * \param compareRelation F function argument
	 * \return F function Bild
	 */
	std::unordered_set<procPair> oneStepBisim(const std::unordered_set<procPair>& compareRelation)
	{
		std::unordered_set<procPair> retMe;
		const auto PROCxPROC = this->returnPROCxPROC();
		auto compareRelationLeftSide = this->getProcsOneSide(e_side::left, compareRelation);
		auto compareRelationRightSide = this->getProcsOneSide(e_side::right, compareRelation);


		for(auto& pP : PROCxPROC)
		{
			/*(p1,p2)
			 *
			 * every transition from p1 must lead to left side, p2 must have a transition existent
			 * every transition from p2 must lead to right side, p1 must have transition existent
			 */

			const Proc* p1 = this->findProcFromId(pP.procOne);
			const Proc* p2 = this->findProcFromId(pP.procTwo);
			if (!p1 || !p2) throw std::exception("One of the processes in the F function wasn't found in the LTS"); // this shouldn't be happening


			bool pairIsValid = true;

			for(auto& transitionP1 : p1->transitions)
			{
				// Does the transition lead to left side?
				const bool foundLeft = compareRelationLeftSide.contains(transitionP1.toProc);
				if (!foundLeft)
				{
					pairIsValid = false;
					break; // not found, is not valid
				}

				// ELSE : found
				pairIsValid = false;
				for(auto& transitionP2 : p2->transitions)
				{
					if(transitionP2.tran == transitionP1.tran)
					{
						const bool foundRight = compareRelationRightSide.contains(transitionP2.toProc);
						if (!foundRight) continue; // not found on the right side, is not valid
						

						// is pair in original relation?
						if (!compareRelation.contains(procPair(transitionP1.toProc, transitionP2.toProc))) continue; // pair not found in compare Relation

						// we found a transition -> next transition from p1!
						pairIsValid = true;
						break;
					}
				}
				if (!pairIsValid) break; // we didnt find an existing part!
			}

			if(pairIsValid)
			for (auto& transitionP2 : p2->transitions)
			{
				// Does the transition lead to right side?
				const bool foundRight = compareRelationRightSide.contains(transitionP2.toProc);
				if (!foundRight)
				{
					pairIsValid = false;
					break; // not found, is not valid
				}

				// ELSE : found
				pairIsValid = false;

				for (auto& transitionP1 : p1->transitions)
				{
					if (transitionP2.tran == transitionP1.tran)
					{
						const bool foundLeft = compareRelationLeftSide.contains(transitionP1.toProc);
						if (!foundLeft) continue; // not found, is not valid

						if (!compareRelation.contains(procPair(transitionP1.toProc, transitionP2.toProc))) continue; // pair not found in compare Relation

						// we found a transition -> next transition from p1!
						pairIsValid = true;
						break;
					}
				}
				if (!pairIsValid) break; // we didnt find an existing part!
			}

			if(pairIsValid)
			{
				retMe.emplace(procPair(p1->procNumber, p2->procNumber));
			}
		}
		return retMe;
	}

	/**
	 * \brief (left,{(1,2),(1,3),(2,4)}) returns {1,2}
	 * \param side "left" or "right"
	 * \param procPairs the processPairs, from which to get all the procs from the specific side
	 * \return array of all the procs of the specific side
	 */
	std::unordered_set<int> getProcsOneSide(const e_side side, const std::unordered_set<procPair>& procPairs)
	{
		std::unordered_set<int> retMe = std::unordered_set<int>();
		if (side == e_side::left)
			for (auto& pair : procPairs)
				retMe.emplace(pair.procOne);
			
		else if (side == e_side::right)
			for (auto& pair : procPairs)
				retMe.emplace(pair.procTwo);

		return retMe;
	}

	/**
 * \brief prints all pairs in ascending order
 * \param procPairs the pairs to print
 * \param printToMe the file to print to
 * \param latexFormat print in format for latex, also with reflexive and symmetric conclusions, if they exist!
 */
	void printProcPairRelation(const std::unordered_set<procPair>& procPairs, FILE* printToMe = nullptr, bool latexFormat = false)
	{
		if (!printToMe) printToMe = fopen("TMPFile.txt", "a+");
		if (!printToMe) throw std::exception("Couldn't open File!");

		std::vector<procPair> sortedPairs = std::vector<procPair>();
		sortedPairs.reserve(procPairs.size());

		for (auto& pair : procPairs)
		{
			sortedPairs.emplace_back(pair);
		}

		std::sort(sortedPairs.begin(), sortedPairs.end());


		if (!latexFormat)
		{
			fprintf(printToMe, "\n{");
			for (const auto& sortedPair : sortedPairs)
				fprintf(printToMe, "(%d, %d), ", sortedPair.procOne, sortedPair.procTwo);
			fprintf(printToMe, "}\n");
		}
		else
		{
			bool isReflexive = true;
			bool isSymmetric = true;

			// check, if all the reflexive pairs are in procPair
			for(auto &proc : this->processes)
			{
				if (!procPairs.contains(procPair(proc.procNumber, proc.procNumber)))
				{
					isReflexive = false;
					break;
				}
			}

			// check, if all the symmetric pairs are in procPair
			for(auto &pP : procPairs)
			{
				if(!procPairs.contains(procPair(pP.procTwo, pP.procOne)))
				{
					isSymmetric = false;
					break;
				}
			}

			fprintf(printToMe, "\n");
			if(isSymmetric) fprintf(printToMe, "s(");
			if (isReflexive) fprintf(printToMe, "r(");
			fprintf(printToMe, "\\{");
			if (!sortedPairs.empty())
			{
				for (unsigned int i = 0; i < sortedPairs.size() - 1; i++)
				{
					const auto& pair = sortedPairs[i];
					if (isReflexive && (pair.procOne == pair.procTwo)) continue;
					if (isSymmetric && (pair.procOne > pair.procTwo)) continue;
					fprintf(printToMe, "(%d, %d), ", pair.procOne, pair.procTwo);
				}
					// last one
					const auto& pair = sortedPairs[sortedPairs.size() - 1];
					if (!((isReflexive && pair.procOne == pair.procTwo) || (isSymmetric && pair.procOne > pair.procTwo)))
						fprintf(printToMe, "(%d, %d)", pair.procOne, pair.procTwo);
			}
			
			fprintf(printToMe, "\\}");
			if (isSymmetric) fprintf(printToMe, ")");
			if (isReflexive) fprintf(printToMe, ")");
			fprintf(printToMe, "\n");
		}


	}


};



void ha007TransitionSystemInPlace(const TranSystem& ts)
{
	ts.addTransition(1, 5, 'c');

	ts.addTransition(2, 2, 'a');
	ts.addTransition(2, 6, 'a');

	ts.addTransition(3, 4, 'a');
	ts.addTransition(3, 4, 'b');
	ts.addTransition(3, 8, 'b');

	ts.addTransition(5, 4, 'c');
	ts.addTransition(5, 9, 'b');

	ts.addTransition(6, 11, 'a');
	ts.addTransition(6, 3, 'd');

	ts.addTransition(7, 8, 'c');
	ts.addTransition(7, 8, 'b');

	ts.addTransition(9, 4, 'a');

	ts.addTransition(10, 5, 'c');
	ts.addTransition(10, 9, 'b');

	ts.addTransition(11, 6, 'a');
	ts.addTransition(11, 13, 'd');


	ts.addTransition(12, 7, 'c');

	ts.addTransition(13, 4, 'a');
	ts.addTransition(13, 8, 'b');

	ts.addTransition(14, 14, 'a');
	ts.addTransition(14, 13, 'd');
}





int main()
{
	TranSystem ts(14);
	ha007TransitionSystemInPlace(ts);

	//vector<procPairSymmetrical> PROCxPROC = ts.returnPROCxPROCSymmetrical();
	FILE* DEBUGFile = fopen("DEBUGFile.txt", "a+");  // NOLINT(clang-diagnostic-deprecated-declarations)
	fprintf(DEBUGFile, "\n-----Opened: -----\n");


	std::unordered_set<procPair> ProcXProc = ts.returnPROCxPROC();
	std::unordered_set<procPair> EmptySet;

	std::unordered_set<procPair> tmp1 = EmptySet; // starting Set

	for(int i = 0; i < 100; i++)
	{
		ts.printProcPairRelation(tmp1, DEBUGFile, true);
		auto tmp2 = ts.oneStepBisim(tmp1);
		if(tmp2.size() == tmp1.size())
		{
			bool fixpoint = true;
			for(auto& p2 : tmp2)
			{
				if(!tmp1.contains(p2))
				{
					fixpoint = false;
					break;
				}
			}
			if(fixpoint)
			{
				fprintf(DEBUGFile, "\nFIXPOINT REACHED!\n");
				break;
			}
		}
		tmp1 = tmp2;
	}

	fprintf(DEBUGFile, "\n-----Closed! ----- \n");
	fclose(DEBUGFile);

	return 0;
}
