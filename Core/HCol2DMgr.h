#pragma once
#include <set>
#include <unordered_map>
#include <list>
#include "DXContainer.h"
#include "HCol2DBase.h"

class State;
class HCol2DMgr
{
public:
	friend State;

private:
	std::set<__int64>::iterator StartColIter;
	std::set<__int64>::iterator EndColIter;
	std::set<__int64> m_Link;

	std::unordered_map<int, std::list<KPtr<HColCom>>>::iterator LeftGIter;
	std::unordered_map<int, std::list<KPtr<HColCom>>>::iterator RightGIter;
	std::unordered_map<int, std::list<KPtr<HColCom>>> m_2DColMap;

	std::list<KPtr<HColCom>>::iterator LeftStartIter;
	std::list<KPtr<HColCom>>::iterator LeftEndIter;
	std::list<KPtr<HColCom>>::iterator RightStartIter;
	std::list<KPtr<HColCom>>::iterator RightEndIter;

public:
	void Link(int _Value);
	void Link(int _Left, int Right);
	void Push2DCol(KPtr<HColCom> _Col);
	std::list<KPtr<HColCom>> AllUpdateColCheck(int _Order, const HColFi* _pColFi);
	KPtr<HColCom> UpdateColCheck(int _Order, const HColFi* _pColFi);

private:
	void Progress();
	void Release();

private:
	void PushOverCol2D(KPtr<HCol2DBase> _Renderer);

public:
	HCol2DMgr();
	~HCol2DMgr();
};

