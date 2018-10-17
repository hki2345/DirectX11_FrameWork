#pragma once
#include <set>
#include <unordered_map>
#include <list>
#include "DHeader.h"
#include "HCol2DBase.h"

class HScene;
class HCol2DMgr
{
public:
	friend HScene;

private:
	std::set<__int64>::iterator StartColIter;
	std::set<__int64>::iterator EndColIter;
	std::set<__int64> m_Link;

	std::unordered_map<int, std::list<HPTR<HColCom>>>::iterator LeftGIter;
	std::unordered_map<int, std::list<HPTR<HColCom>>>::iterator RightGIter;
	std::unordered_map<int, std::list<HPTR<HColCom>>> m_2DColMap;

	std::list<HPTR<HColCom>>::iterator LeftStartIter;
	std::list<HPTR<HColCom>>::iterator LeftEndIter;
	std::list<HPTR<HColCom>>::iterator RightStartIter;
	std::list<HPTR<HColCom>>::iterator RightEndIter;

public:
	void Link(int _Value);
	void Link(int _Left, int Right);
	void Push2DCol(HPTR<HColCom> _Col);
	std::list<HPTR<HColCom>> AllUpdateColCheck(int _Order, const HColFi* _pColFi);
	HPTR<HColCom> UpdateColCheck(int _Order, const HColFi* _pColFi);

private:
	void Progress();
	void Release();

private:
	void PushOverCol2D(HPTR<HCol2DBase> _Renderer);

public:
	HCol2DMgr();
	~HCol2DMgr();
};

