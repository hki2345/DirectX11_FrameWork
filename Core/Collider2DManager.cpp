#include "Collider2DManager.h"
#include "KMacro.h"


Collider2DManager::Collider2DManager()
{
}


Collider2DManager::~Collider2DManager()
{
}

void Collider2DManager::progress()
{
	S_Col_Iter = m_Chain.begin();
	E_Col_Iter = m_Chain.end();

	for (; S_Col_Iter != E_Col_Iter; ++S_Col_Iter)
	{
		int_compare NewChain = (*S_Col_Iter);

		L_Iter = m_Collider2D_Map.find(NewChain.Left);
		R_Iter = m_Collider2D_Map.find(NewChain.Right);

		// 링크 된 게 없으면 하지 않겠다.
		if (0 >= L_Iter->second.size() || 0 >= R_Iter->second.size())
		{
			continue;
		}

		// 왼쪽 혹은 오른쪽 중 하나라도 충돌체가 없으면 하지 않겠다.
		if (L_Iter == m_Collider2D_Map.end() || R_Iter == m_Collider2D_Map.end())
		{
			continue;
		}

		LS_Iter = L_Iter->second.begin();
		LE_Iter = L_Iter->second.end();
		for (; LS_Iter != LE_Iter; ++LS_Iter)
		{
			(*LS_Iter)->Update_Collision();
		}

		RS_Iter = R_Iter->second.begin();
		RE_Iter = R_Iter->second.end();
		for (; RS_Iter != RE_Iter; ++RS_Iter)
		{
			(*RS_Iter)->Update_Collision();
		}

		//if (NewChain.Left == NewChain.Right)
		//{

		//}
		//else
		{
			LS_Iter = L_Iter->second.begin();
			LE_Iter = L_Iter->second.end();

			for (; LS_Iter != LE_Iter; ++LS_Iter)
			{
				// 활성화 안 되어있으면 당연 안해야 것지
				if (false == (*LS_Iter)->is_Active())
				{
					continue;
				}

				RS_Iter = R_Iter->second.begin();
				RE_Iter = R_Iter->second.end();

				for (; RS_Iter != RE_Iter; ++RS_Iter)
				{
					if (false == (*RS_Iter)->is_Active() || (*LS_Iter) == (*RS_Iter))
					{
						continue;
					}

					(*LS_Iter)->check((*RS_Iter));
				}
			}
		}
	}
}


void Collider2DManager::insert_AbsCollider2D(KPtr<Collider2D> _Col)
{
	KASSERT(nullptr == _Col);

	std::unordered_map<int, std::list<KPtr<Collision>>>::iterator AbsFindIter = m_Collider2D_Map.find(_Col->m_Order);

	if (AbsFindIter == m_Collider2D_Map.end())
	{
		m_Collider2D_Map.insert(std::unordered_map<int, std::list<KPtr<Collision>>>::
			value_type(_Col->m_Order, std::list<KPtr<Collision>>()));
		AbsFindIter = m_Collider2D_Map.find(_Col->m_Order);
	}

	AbsFindIter->second.push_back(_Col);
}

void Collider2DManager::chain(const int& _Value)
{
	chain(_Value, _Value);
}

void Collider2DManager::chain(const int& _Left, const int& _Right)
{
	int_compare NewChain = { _Left, _Right };

	std::unordered_map<int, std::list<KPtr<Collision>>>::iterator Find_Iter;

	Find_Iter = m_Collider2D_Map.find(_Left);
	if (Find_Iter == m_Collider2D_Map.end())
	{
		m_Collider2D_Map.insert(std::unordered_map<int, std::list<KPtr<Collision>>>::
			value_type(_Left, std::list<KPtr<Collision>>()));
	}

	Find_Iter = m_Collider2D_Map.find(_Right);
	if (Find_Iter == m_Collider2D_Map.end())
	{
		m_Collider2D_Map.insert(std::unordered_map<int, std::list<KPtr<Collision>>>::
			value_type(_Right, std::list<KPtr<Collision>>()));
	}

	std::set<__int64>::iterator Chain_Iter = m_Chain.find(NewChain);

	if (Chain_Iter != m_Chain.end())
	{
		return;
	}

	m_Chain.insert(NewChain);
}

void Collider2DManager::insert_2D(KPtr<Collision> _Col)
{
	if (nullptr == _Col)
	{
		return;
	}

	std::unordered_map<int, std::list<KPtr<Collision>>>::iterator Find_Iter = m_Collider2D_Map.find(_Col->m_Order);

	if (Find_Iter == m_Collider2D_Map.end())
	{
		m_Collider2D_Map.insert(std::unordered_map<int, std::list<KPtr<Collision>>>::
			value_type(_Col->m_Order, std::list<KPtr<Collision>>()));
		Find_Iter = m_Collider2D_Map.find(_Col->m_Order);
	}

	Find_Iter->second.push_back(_Col);
}


KPtr<Collision> Collider2DManager::All_Collision_Check(Collision* _Other)
{
	std::unordered_map<int, std::list<KPtr<Collision>>>::iterator S_UI = m_Collider2D_Map.begin();
	std::unordered_map<int, std::list<KPtr<Collision>>>::iterator E_UI = m_Collider2D_Map.end();
	
	for (; S_UI != E_UI; ++S_UI)
	{
		std::list<KPtr<Collision>>::iterator ULCStartIter = S_UI->second.begin();
		std::list<KPtr<Collision>>::iterator ULCEndIter = S_UI->second.end();

		for (; ULCStartIter != ULCEndIter; ++ULCStartIter)
		{
			if ((*ULCStartIter) == _Other)
			{
				continue;
			}

			if (false == (*ULCStartIter)->one()->is_Active())
			{
				continue;
			}

			if (true == (*ULCStartIter)->collision_check(_Other))
			{
				return (*ULCStartIter);
			}
			// (*LeftStartIter)->FiColCheck();
		} // for (; LeftStartIter != LeftEndIter; ++LeftStartIter)
	}
	return nullptr;
}

KPtr<Collision> Collider2DManager::Collision_Check(const int& _Order, Collision* _Other)
{
	std::unordered_map<int, std::list<KPtr<Collision>>>::iterator UpdateCheckIter = m_Collider2D_Map.find(_Order);

	if (UpdateCheckIter == m_Collider2D_Map.end())
	{
		return nullptr;
	}

	std::list<KPtr<Collision>>::iterator ULCStartIter = UpdateCheckIter->second.begin();
	std::list<KPtr<Collision>>::iterator ULCEndIter = UpdateCheckIter->second.end();

	for (; ULCStartIter != ULCEndIter; ++ULCStartIter)
	{
		if ((*ULCStartIter) == _Other)
		{
			continue;
		}

		if (false == (*ULCStartIter)->one()->is_Active())
		{
			continue;
		}

		if (true == (*ULCStartIter)->collision_check(_Other))
		{
			return (*ULCStartIter);
		}
		// (*LeftStartIter)->FiColCheck();
	} // for (; LeftStartIter != LeftEndIter; ++LeftStartIter)

	return nullptr;
}

void Collider2DManager::release()
{
	std::unordered_map<int, std::list<KPtr<Collision>>>::iterator StartIter = m_Collider2D_Map.begin();
	std::unordered_map<int, std::list<KPtr<Collision>>>::iterator EndIter = m_Collider2D_Map.end();

	std::list<KPtr<Collision>>::iterator ListStartIter;
	std::list<KPtr<Collision>>::iterator ListEndIter;

	for (; StartIter != EndIter; ++StartIter)
	{
		ListStartIter = StartIter->second.begin();
		ListEndIter = StartIter->second.end();

		for (; ListStartIter != ListEndIter;)
		{
			if (true == (*ListStartIter)->is_Death())
			{
				(*ListStartIter)->death_release();
				ListStartIter = StartIter->second.erase(ListStartIter);
			}
			else {
				++ListStartIter;
			}
		}
	}

}
