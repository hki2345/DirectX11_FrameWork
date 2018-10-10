#include "StateManager.h"
#include "Stl_Assistor.h"
#include "KMacro.h"
#include "KWindow.h"
#include "DebugManager.h"



bool StateManager::find_absOne(KPtr<TheOne> _Other)
{
	std::list<KPtr<TheOne>>::iterator S_AbsOneIter = m_AbsOneList.begin();
	std::list<KPtr<TheOne>>::iterator E_AbsOneIter = m_AbsOneList.end();

	for (; S_AbsOneIter != E_AbsOneIter; ++S_AbsOneIter)
	{
		if ((*S_AbsOneIter) == _Other)
		{
			return true;
		}
	}

	return false;
}



void StateManager::abs_check(State* _Other)
{
	std::list<KPtr<TheOne>>::iterator S_AbsOneIter = m_AbsOneList.begin();
	std::list<KPtr<TheOne>>::iterator E_AbsOneIter = m_AbsOneList.end();

	for (; S_AbsOneIter != E_AbsOneIter; ++S_AbsOneIter)
	{
		if (OBJ_ABSTYPE::NONE_ABS != (*S_AbsOneIter)->abs_type())
		{
			continue;
		}
		_Other->insert_AbsOne(*S_AbsOneIter);
	}
}



StateManager::StateManager(KWindow* _WIndow)
{
	window(_WIndow);
}


StateManager::~StateManager()
{
}

KPtr<State> StateManager::change_state(const wchar_t* _Name)
{
	// 일부러 null을 넣어 에딧 씬에서 편집할 수 있게함
	if (nullptr == _Name)
	{
		m_NextState = nullptr;
		return nullptr;
	}

	if (nullptr != m_CurState)
	{
		m_CurState->End();
	}
	m_NextState = find_state(_Name);
	KASSERT(nullptr == m_NextState);

	return m_NextState;
}

KPtr<State> StateManager::find_state(const wchar_t* _Name)
{
	return Map_Find<KPtr<State>>(m_StateMap, _Name);
}

void StateManager::Progress()
{
	if (nullptr != m_NextState)
	{
		m_CurState = m_NextState;
		m_NextState = nullptr;

		if (false == m_CurState->Is_Build())
		{
			m_CurState->Build();
		}

		m_CurState->Start();
	}

	if (nullptr != m_CurState && true == m_CurState->is_Active())
	{
		m_CurState->Progress();
	}
}

void StateManager::Next_Progress()
{
	if (nullptr != m_CurState && true == m_CurState->is_Active())
	{
		m_CurState->Next_Progress();
	}
}

void StateManager::Render()
{
	if (nullptr != m_CurState && true == m_CurState->is_Active())
	{
		m_CurState->Render();
	}
}


void StateManager::Release()
{
	if (nullptr != m_CurState && true == m_CurState->is_Active())
	{
		m_CurState->Release();
	}
}

KPtr<State> StateManager::create_state(const wchar_t* _Name)
{
	if ( nullptr != find_state(_Name))
	{
		return nullptr;
	}

	KPtr<State> NewScene = new State();
	NewScene->statemanager(this);
	NewScene->name(_Name);
	NewScene->window(window());
	NewScene->set_type();

	abs_check(NewScene);

	m_StateMap.insert(std::unordered_map<std::wstring, KPtr<State>>::value_type(_Name, NewScene));
	return NewScene;

}


