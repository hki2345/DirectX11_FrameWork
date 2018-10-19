#include "HCol2DMgr.h"
#include "KMacro.h"

HCol2DMgr::HCol2DMgr()
{
}


HCol2DMgr::~HCol2DMgr()
{
}

void HCol2DMgr::Progress() 
{
	StartColIter = m_Link.begin();
	EndColIter = m_Link.end();

	for (; StartColIter != EndColIter; ++StartColIter)
	{
		// 
		HIndex Link = (*StartColIter);

		LeftGIter = m_2DColMap.find(Link.Left);
		RightGIter = m_2DColMap.find(Link.Right);

		if (LeftGIter == m_2DColMap.end() || RightGIter == m_2DColMap.end())
		{
			continue;
		}

		if (0 >= LeftGIter->second.size() || 0 >= RightGIter->second.size())
		{
			continue;
		}

		LeftStartIter = LeftGIter->second.begin();
		LeftEndIter = LeftGIter->second.end();
		for (; LeftStartIter != LeftEndIter; ++LeftStartIter)
		{
			(*LeftStartIter)->ColFiUpdate();
		}

		RightStartIter = RightGIter->second.begin();
		RightEndIter = RightGIter->second.end();
		for (; RightStartIter != RightEndIter; ++RightStartIter)
		{
			(*RightStartIter)->ColFiUpdate();
		}

		if (Link.Left == Link.Right)
		{

		}
		else { // 
			LeftStartIter = LeftGIter->second.begin();
			LeftEndIter = LeftGIter->second.end();

			for (; LeftStartIter != LeftEndIter; ++LeftStartIter)
			{
				if (false == (*LeftStartIter)->Is_Active())
				{
					continue;
				}

				RightStartIter = RightGIter->second.begin();
				RightEndIter = RightGIter->second.end();

				for (; RightStartIter != RightEndIter; ++RightStartIter)
				{
					if (false == (*RightStartIter)->Is_Active())
					{
						continue;
					}

					(*LeftStartIter)->ColCheck((*RightStartIter));
				} // for (; RightStartIter != RightEndIter; ++RightStartIter)
			} // for (; LeftStartIter != LeftEndIter; ++LeftStartIter)
		}
	}

}
void HCol2DMgr::Push2DCol(KPtr<HColCom> _Col)
{
	if (nullptr == _Col)
	{
		return;
	}

	std::unordered_map<int, std::list<KPtr<HColCom>>>::iterator FindIter = m_2DColMap.find(_Col->m_Order);

	if (FindIter == m_2DColMap.end())
	{
		m_2DColMap.insert(std::unordered_map<int, std::list<KPtr<HColCom>>>::value_type(_Col->m_Order, std::list<KPtr<HColCom>>()));
		FindIter = m_2DColMap.find(_Col->m_Order);
	}

	FindIter->second.push_back(_Col);
}
void HCol2DMgr::Link(int _Value)
{
	Link(_Value, _Value);
}
void HCol2DMgr::Link(int _Left, int _Right) 
{
	HIndex Index = { _Left , _Right };

	std::unordered_map<int, std::list<KPtr<HColCom>>>::iterator FindIter;
	FindIter = m_2DColMap.find(_Left);
	if (FindIter == m_2DColMap.end())
	{
		m_2DColMap.insert(std::unordered_map<int, std::list<KPtr<HColCom>>>::value_type(_Left, std::list<KPtr<HColCom>>()));
	}

	FindIter = m_2DColMap.find(_Right);
	if (FindIter == m_2DColMap.end())
	{
		m_2DColMap.insert(std::unordered_map<int, std::list<KPtr<HColCom>>>::value_type(_Right, std::list<KPtr<HColCom>>()));
	}

	std::set<__int64>::iterator Iter = m_Link.find(Index);

	if (Iter != m_Link.end())
	{
		return;
	}

	m_Link.insert(Index);
}

void HCol2DMgr::PushOverCol2D(KPtr<HCol2DBase> _Collision) 
{
	KASSERT(nullptr == _Collision);

	std::unordered_map<int, std::list<KPtr<HColCom>>>::iterator FindGIter = m_2DColMap.find(_Collision->m_Order);

	if (FindGIter == m_2DColMap.end())
	{
		m_2DColMap.insert(std::unordered_map<int, std::list<KPtr<HColCom>>>::value_type(_Collision->m_Order, std::list<KPtr<HColCom>>()));
		FindGIter = m_2DColMap.find(_Collision->m_Order);
	}

	FindGIter->second.push_back(_Collision);
}

std::list<KPtr<HColCom>> HCol2DMgr::AllUpdateColCheck(int _Order, const HColFi* _pColFi)
{
	std::list<KPtr<HColCom>> ReturnList;

	std::unordered_map<int, std::list<KPtr<HColCom>>>::iterator UpdateCheckIter = m_2DColMap.find(_Order);

	if (UpdateCheckIter == m_2DColMap.end())
	{
		return ReturnList;
	}

	std::list<KPtr<HColCom>>::iterator ULCStartIter = UpdateCheckIter->second.begin();
	std::list<KPtr<HColCom>>::iterator ULCEndIter = UpdateCheckIter->second.end();

	for (; ULCStartIter != ULCEndIter; ++ULCStartIter)
	{
		if (true == (*ULCStartIter)->FiColCheck(_pColFi))
		{
			ReturnList.push_back((*ULCStartIter));
		}
		// (*LeftStartIter)->FiColCheck();
	} // for (; LeftStartIter != LeftEndIter; ++LeftStartIter)

	return ReturnList;
}

KPtr<HColCom> HCol2DMgr::UpdateColCheck(int _Order, const HColFi* _pColFi)
{
	std::unordered_map<int, std::list<KPtr<HColCom>>>::iterator UpdateCheckIter = m_2DColMap.find(_Order);

	if (UpdateCheckIter == m_2DColMap.end())
	{
		return nullptr;
	}

	std::list<KPtr<HColCom>>::iterator ULCStartIter = UpdateCheckIter->second.begin();
	std::list<KPtr<HColCom>>::iterator ULCEndIter = UpdateCheckIter->second.end();

	for (; ULCStartIter != ULCEndIter; ++ULCStartIter)
	{
		if (true == (*ULCStartIter)->FiColCheck(_pColFi))
		{
			return (*ULCStartIter);
		}
		// (*LeftStartIter)->FiColCheck();
	} // for (; LeftStartIter != LeftEndIter; ++LeftStartIter)

	return nullptr;
}

void HCol2DMgr::Release() 
{
	std::unordered_map<int, std::list<KPtr<HColCom>>>::iterator StartIter = m_2DColMap.begin();
	std::unordered_map<int, std::list<KPtr<HColCom>>>::iterator EndIter = m_2DColMap.end();

	std::list<KPtr<HColCom>>::iterator ListStartIter;
	std::list<KPtr<HColCom>>::iterator ListEndIter;

	for (; StartIter != EndIter; ++StartIter)
	{
		ListStartIter = StartIter->second.begin();
		ListEndIter = StartIter->second.end();

		for (; ListStartIter != ListEndIter;)
		{
			if (true == (*ListStartIter)->Is_Death())
			{
				(*ListStartIter)->DeathRelease();
				ListStartIter = StartIter->second.erase(ListStartIter);
			}
			else {
				++ListStartIter;
			}
		}
	}

}