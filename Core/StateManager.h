#pragma once
#include <unordered_map>
#include <string>
#include "State.h"
#include "Mother_KWindow.h"


class KWindow;
class TheOne;
class StateManager : public Mother_KWindow
{
public: 
	friend TheOne;
	friend KWindow;


private:
	// 절대적 존재 - 어느 스테이트에서도 존재하는 넘
	std::list<KPtr<TheOne>> m_AbsOneList;

public:
	bool find_absOne(KPtr<TheOne> _Other);
	void abs_check(State* _NewScene);
	KPtr<State> state() { return m_CurState; }
	KPtr<State> next_state() { return m_NextState; }

private:
	std::unordered_map <std::wstring, KPtr<State>> m_StateMap;
	std::unordered_map <std::wstring, KPtr<State>>::iterator S_StateMap;
	std::unordered_map <std::wstring, KPtr<State>>::iterator E_StateMap;


	KPtr<State> m_CurState;
	KPtr<State> m_NextState;

public:
	template<typename Builder, typename Updater>
	KPtr<State> create_state(const wchar_t*_Name, bool _Build = true)
	{
		if (nullptr != find_state(_Name))
		{
			return nullptr;
		}

		KPtr<State> NewState = new State();

		NewState->statemanager(this);
		NewState->name(_Name);
		NewState->state(NewState);
		NewState->window(window());
		NewState->set_type();

		abs_check(NewState);

		NewState->Create_Builder<Builder>();
		NewState->Create_Updater<Updater>();

		if (true == _Build)
		{
			NewState->Build();
		}

		m_StateMap.insert(std::unordered_map<std::wstring, KPtr<State>>::value_type(_Name, NewState));
		return NewState;
	}

	KPtr<State> create_state(const wchar_t* _Name);

	KPtr<State> find_state(const wchar_t* _Name);
	KPtr<State> change_state(const wchar_t* _Name);

private:
	void Progress();
	void Render();
	void Next_Progress();
	void Release();

public:
	StateManager(KWindow* _WIndow);
	~StateManager();
};

