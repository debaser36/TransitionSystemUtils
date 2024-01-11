#include "procSystem.h"

#include <algorithm>
#include <fstream>
#include <utility>

Proc::Proc(std::string ID) : m_ID(std::move(ID))
{
}

const std::string& Proc::getID() const
{
	return this->m_ID;
}

ProcPair::ProcPair(const Proc& _p1, const Proc& _p2) : p1(&_p1), p2(&_p2)
{
}

ProcPair::ProcPair(const Proc* _p1, const Proc* _p2) : p1(_p1), p2(_p2)
{
	if (!this->p1 || !this->p2) throw std::exception("One Process in ProcPair is NULL!");
}

Action::Action(std::string ID) : m_TranName(std::move(ID))
{
}

const std::string& Action::getID() const
{
	return this->m_TranName;
}
Transition::Transition(const Proc& from, const Proc& to, const Action& tran) : procFrom(&from), procTo(&to), actionName(&tran)
{
}

Transition::Transition(const Proc* from, const Proc* to, const Action* tran) : procFrom(from), procTo(to), actionName(tran)
{
}

Transition::Transition(const std::string& fromID, const std::string& toID, const std::string& tranID) : procFrom(new Proc(fromID)), procTo(new Proc(toID)), actionName(new Action(tranID))
{
}



std::string Transition::TransitionString() const
{
	return "(" + this->procFrom->getID() + ") --" + this->actionName->getID() + "-> " + this->procTo->getID() + ")";
}

size_t Transition::compareValue() const
{
	size_t counter = this->actionName->getID().length() + this->procFrom->getID().length() + this->procTo->getID().length() - 1;
	size_t val = 0;
	for(auto& c: this->procFrom->getID())
	{
		val += (counter*42) * (c-48);
		counter--;
	}
	for(auto& c : this->procTo->getID())
	{
		val += (counter * 42) * (c - 48);
		counter--;
	}
	for(auto& c : this->actionName->getID())
	{
		val += (counter * 42) * (c - 48);
		counter--;
	}
	return val;
}

const Proc* Transition::getProcFrom() const
{
	return this->procFrom;
}

const Proc* Transition::getProcTo() const
{
	return this->procTo;
}

const Action* Transition::getAction() const
{
	return this->actionName;
}


LTS::LTS(int procCountMAX, int actCountMAX) : m_Proc(std::vector<Proc>()), m_Act(std::vector<Action>()), m_Tran(std::vector<Transition>())
{
	m_Proc.reserve(procCountMAX);
	m_Act.reserve(actCountMAX);
}

LTS::LTS(std::vector<Proc>& procs, std::vector<Action>& acts, std::vector<Transition>& trans) : m_Proc(std::move(procs)), m_Act(std::move(acts)), m_Tran(std::move(trans))
{
}

void LTS::addProc(const std::string& procToAdd)
{
	const auto foundProc = findProc(procToAdd);
	if(!foundProc)
	{
		procXProcIsValid = false;
		this->m_Proc.emplace_back(procToAdd);
	}
}

void LTS::addAct(const std::string& actToAdd)
{
	const auto foundAct = findAct(actToAdd);
	if(!foundAct)
		this->m_Act.emplace_back(actToAdd);
}

void LTS::addTransition(const std::string& from, const std::string& to, const std::string& act)
{
	// TODO doubled search - inefficient, but not dramatic
	this->addProc(from);
	this->addProc(to);
	this->addAct(act);
	const auto p1 = findProc(from);
	const auto p2 = findProc(to);
	const auto a = findAct(act);
	this->m_Tran.emplace_back(*p1, *p2, *a);
}

const Proc* LTS::findProc(const std::string& procToFind) const
{
	for(auto& proc : this->m_Proc)
	{
		if (proc.getID() == procToFind) return &proc;
	}
	return nullptr;
}

const Action* LTS::findAct(const std::string& actToFind) const
{
	for (auto& act : this->m_Act)
	{
		if (act.getID() == actToFind) return &act;
	}
	return nullptr;
}

const std::vector<Transition>& LTS::getAllTransitions()
{
	return this->m_Tran;
}

const std::vector<Proc>& LTS::getAllProcs()
{
	return this->m_Proc;
}

void LTS::calculateProcXProc()
{
	if (procXProcIsValid) return;
	ProcXProc.clear();
	for (auto& p1 : this->m_Proc)
		for (auto& p2 : this->m_Proc)
			this->ProcXProc.emplace_back(p1, p2);
	procXProcIsValid = true;
}

void LTS::sortTrans()
{

	 std::ranges::sort(this->m_Tran,
		[](const Transition& a, const Transition& b)
		{
			/*const std::string aStr = a.TransitionString();
			const std::string bStr = b.TransitionString();
			return aStr < bStr; */ 
			return a.compareValue() < b.compareValue();
		});
	

	procXProcIsValid = false;
	calculateProcXProc();
}

std::vector<Transition*> LTS::getTransitionsFromProc(const Proc& proc)
{
	std::vector<Transition*> retMe;
	for(auto& transition : this->m_Tran)
	{
		if(&proc == (transition.getProcFrom()))
		{
			retMe.emplace_back(&transition);
		}
	}
	return retMe;
}


std::vector<ProcPair> bigF(const std::vector<ProcPair>& inputRelation, LTS& lts)
{
	std::vector<ProcPair> retMe;
	lts.calculateProcXProc();
	const auto& PROCxPROC = lts.ProcXProc;


	for (auto& pP : PROCxPROC)
	{
		/*(p1,p2)
		 *
		 * every transition from p1 must lead to left side, p2 must have a transition existent
		 * every transition from p2 must lead to right side, p1 must have transition existent
		 #1# */

		const Proc& p1 = *pP.p1;
		const Proc& p2 = *pP.p2;
		bool pairIsValid = true;

		auto p1transitions = lts.getTransitionsFromProc(p1);
		auto p2transitions = lts.getTransitionsFromProc(p2);



		for (const auto& transitionP1 : p1transitions)
		{
			// ELSE : found
			pairIsValid = false;
			for (const auto& transitionP2 : p2transitions)
			{
				if (transitionP2->getAction()->getID() == transitionP1->getAction()->getID())
				{
					// is pair in original relation?
					for (auto& pairInInput : inputRelation)
					{
						if (pairInInput.p1->getID() == transitionP1->getProcTo()->getID() && pairInInput.p2->getID() == transitionP2->getProcTo()->getID())
						{
							pairIsValid = true;
							break;
						}
					}
					if (pairIsValid) break;
					// we found a transition -> next transition from p1!
				}
			}
			if (!pairIsValid) break; // we didn't find an existing part!
		}

		if (pairIsValid)
			for (const auto& transitionP2 : p2transitions)
			{
				pairIsValid = false;

				for (const auto& transitionP1 : p1transitions)
				{
					if (transitionP2->getAction()->getID() == transitionP1->getAction()->getID())
					{
						// is pair in original relation?
						for (auto& pairInInput : inputRelation)
						{
							if (pairInInput.p1->getID() == transitionP1->getProcTo()->getID() && pairInInput.p2->getID() == transitionP2->getProcTo()->getID())
							{
								pairIsValid = true;
								break;
							}
						}
						if (pairIsValid) break;
						// we found a transition -> next transition from p1!
					}
				}
				if (!pairIsValid) break; // we didn't find an existing part!
			}

		if (pairIsValid)
		{
			retMe.emplace_back(p1, p2);
		}
	}
	return retMe;
}

std::vector<ProcPair> getBisimilarity(LTS& lts)
{
	lts.calculateProcXProc();
	auto& startingPoint = lts.ProcXProc;
	while(true)
	{
		std::vector<ProcPair> result = bigF(startingPoint, lts);
		// JUST TESTING bool d = findInRelation("q1", "q3", result);
		if(result.size() == startingPoint.size())
		{
			startingPoint = result;
			break;
		}
		startingPoint = result;
	}
	std::ofstream debugFile;
	debugFile.open("DebugFile.txt", std::ios::out |std::ios::trunc);
	debugFile << "This is a test";
	debugFile.close();

	return startingPoint;
}

void printProcProcRelation(const std::vector<ProcPair>& relation, std::ofstream& file, LTS* lts)
{
	if(!file.is_open()) return;
	bool isReflexive = true;
	bool isSymmetric = true;
	if(lts)
	{
		for(auto& proc : lts->getAllProcs())
		{
			bool found = false;
			for(auto& pair: relation)
			{
				if(proc.getID() == pair.p1->getID() && proc.getID() == pair.p2->getID())
				{
					found = true;
					break;
				}
			}
			if(!found)
			{
				isReflexive = false;
				break;
			}
		}
	}
	for(auto& pair1: relation)
	{
		bool found = false;
		for(auto& pair2: relation)
		{
			if(pair1.p1->getID() == pair2.p2->getID() && pair1.p2->getID() == pair2.p1->getID())
			{
				found = true;
				break;
			}
		}
		if(!found)
		{
			isSymmetric = false;
			break;
		}
	}

	// finally print
	file << "\n";
	if(isReflexive) file <<"r(";
	if(isSymmetric) file << "s(";
	file << "{";


	std::vector<int> indicesToPrint;
	indicesToPrint.reserve(relation.size());
	for(size_t i = 0; i < relation.size() - 1; i++)
	{
		auto& pair = relation[i];
		if((isSymmetric && pair.p1->getID() > pair.p2->getID())
			|| (isReflexive && pair.p1->getID() == pair.p2->getID())) 
		{
			// we don't need to push that index
		}
		else indicesToPrint.emplace_back(i);
	}

	for(size_t i = 0; i < indicesToPrint.size() - 1; i++)
	{
		auto& pP = relation[indicesToPrint[i]];
		if((isSymmetric && pP.p1->getID() > pP.p2->getID())
			|| (isReflexive && pP.p1->getID() == pP.p2->getID())) continue;
		file << pP << ", ";
	}
	auto& pP = relation[indicesToPrint[indicesToPrint.size() - 1]];

	file << pP;
	file << "}";
	if(isReflexive) file << ")";
	if(isSymmetric) file << ")";
	file << "\n";
}

bool findInRelation(const std::string& p1, const std::string& p2, const std::vector<ProcPair>& relation)
{
	for(auto& pair: relation)
	{
		if(pair.p1->getID() == p1 && pair.p2->getID() == p2) return true;
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const ProcPair& procPair)
{
	os << "(" << procPair.p1->getID() << ", " << procPair.p2->getID() << ")";
	return os;
}
