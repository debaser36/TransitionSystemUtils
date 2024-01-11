#pragma once
#include <string>
#include <vector>

class Proc
{
	std::string m_ID;

public:
	Proc() = delete;
	Proc(std::string ID);

	[[nodiscard]] const std::string& getID() const;
};

class ProcPair
{
public:
	const Proc* p1;
	const Proc* p2;

public:
	ProcPair() = delete;
	ProcPair(const Proc& _p1, const Proc& _p2);
	ProcPair(const Proc* _p1, const Proc* _p2);
	friend std::ostream& operator<<(std::ostream& os, const ProcPair& procPair);
};

class Action
{
	std::string m_TranName;
public:
	Action() = delete;
	Action(std::string ID);

	[[nodiscard]] const std::string& getID() const;
};

class Transition
{
	const Proc * procFrom;
	const Proc * procTo;
	const Action * actionName;
public:
	Transition() = delete;
	Transition(const Proc& from, const Proc& to, const Action& tran);
	Transition(const Proc* from, const Proc* to, const Action* tran);
	Transition(const std::string& fromID, const std::string& toID, const std::string& tranID);

	/**
	 * \brief 
	 * \return (proc1) -Tran-> (proc2)
	 */
	std::string TransitionString() const;

	size_t compareValue() const;
	const Proc* getProcFrom() const;
	const Proc* getProcTo() const ;
	const Action* getAction() const;
};

class LTS
{
	std::vector<Proc> m_Proc;
	std::vector<Action> m_Act;
	std::vector<Transition> m_Tran;

	bool procXProcIsValid = false;

public:
	LTS() = delete;
	/**
	 * \brief We don't implement security behaviour, when using pointers for the transitions... That means, that it is possible, that the pointers in TRAN aren't valid anymore. Please reserve enough space for your vectors PROC and ACT, so that the pointers will stay valid!
	 * \param procCountMAX don't make too low
	 * \param actCountMAX don't make too low
	 */
	LTS(int procCountMAX, int actCountMAX);
	LTS(std::vector<Proc>& procs, std::vector<Action>& acts, std::vector<Transition>& trans);

	/**
	 * \brief checks, if process is already in Proc, and if not - adds it
	 * \param procToAdd Process to Add with String-ID
	 */
	void addProc(const std::string& procToAdd);
	/**
	 * \brief checks, if Action is already in Act, and if not - adds it
	 * \param actToAdd Action to Add with String-ID
	 */
	void addAct(const std::string& actToAdd);

	/**
	 * \brief 
	 * \param from processID
	 * \param to processID
	 * \param act Action Label
	 */
	void addTransition(const std::string& from, const std::string& to, const std::string& act);

	/**
	 * \brief 
	 * \param procToFind 
	 * \return NULL if not found
	 */
	const Proc* findProc(const std::string& procToFind) const;
	/**
	 * \brief 
	 * \param actToFind 
	 * \return NULL if not found
	 */
	const Action* findAct(const std::string& actToFind) const;
	const std::vector<Transition>& getAllTransitions();
	const std::vector<Proc>& getAllProcs();
	void calculateProcXProc();
	void sortTrans();
	std::vector<Transition*> getTransitionsFromProc(const Proc& proc);


	// PUBLIC ACCESS
	std::vector<ProcPair> ProcXProc = std::vector<ProcPair>();

	
};


std::vector<ProcPair> bigF(const std::vector<ProcPair>& inputRelation, LTS& lts);
std::vector<ProcPair> getBisimilarity(LTS& lts);
/**
 * \brief prints
 * \param relation relation to print
 * \param file file to printTo
 * \param lts if given (!= nullptr), this will check for symmetric and reflexive endings, otherwise only symmetric
 */
void printProcProcRelation(const std::vector<ProcPair>& relation, std::ofstream& file, LTS * lts = nullptr);
/**
 * \brief 
 * \param p1 name of process 1
 * \param p2 name of process 2
 * \param relation (p1,p2) in relation ??
 * \return true, if found, false otherwise
 */
bool findInRelation(const std::string& p1, const std::string& p2, const std::vector<ProcPair>& relation);