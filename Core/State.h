#pragma once
#include "Begin_Updater.h"

#include <unordered_map>
#include <list>

#include "Mother_KWindow.h"
#include "Mother_State.h"


#include "RenderManager.h"
#include "Collider2DManager.h"

#include "ReadStream.h"

class TheOne;
class StateManager;
class Camera;

class Edit_Updater : public TypeBegin, public Mother_KWindow, public Mother_State
{
public:
	friend State;

public:
	virtual void Update_State() {};
	virtual void Start_State() {};
	virtual void Debug_Render() {};

protected:
	Edit_Updater() {}
	~Edit_Updater() {}
};

class State : public Begin_Updater, public Mother_KWindow, public Mother_State
{
private:
	friend TheOne;
	friend StateManager;
	friend class RenderManager;

private:
	void insert_AbsOne(KPtr<TheOne> _Other);

#pragma region Updater & Builder
public:
	class State_Updater: public TypeBegin, public Mother_KWindow, public Mother_State
	{
	public:
		friend State;
		
	public:
		virtual void Update_State() {};
		virtual void Start_State() {};
		virtual void End_State() {};
		virtual void UI_Render() {};
		virtual void Debug_Render() {};

	protected:
		State_Updater() {}
		~State_Updater() {}
	};

	class State_Builder : public TypeBegin, public Mother_KWindow, public Mother_State
	{
	public:
		friend State;

	public:
		virtual void Build_State() = 0;

	protected:
		State_Builder() {}
		~State_Builder() {}
	};

private:
	KPtr<State_Builder> m_kp_Builder;
	std::list<KPtr<State_Updater>> m_StateUpdateList;

public:
	void Updater(KPtr<State_Updater> _Updater, const int& _index)
	{
		std::list<KPtr<State_Updater>>::iterator S_UpdaterIter = m_StateUpdateList.begin();
		std::list<KPtr<State_Updater>>::iterator E_UpdaterIter = m_StateUpdateList.end();
		
		int ind = 0;

		for (; S_UpdaterIter != E_UpdaterIter; ++S_UpdaterIter)
		{
			if (ind == _index)
			{
				(*S_UpdaterIter) = _Updater;
				return;
			}
			else
			{
				ind++;
			}
		}
	}

	KPtr<State_Updater> Updater(const int& _index)
	{
		std::list<KPtr<State_Updater>>::iterator S_UpdaterIter = m_StateUpdateList.begin();
		std::list<KPtr<State_Updater>>::iterator E_UpdaterIter = m_StateUpdateList.end();

		int ind = 0;

		for (; S_UpdaterIter != E_UpdaterIter; ++S_UpdaterIter)
		{
			if (ind == _index)
			{
				return (*S_UpdaterIter);
			}
			else
			{
				ind++;
			}
		}
	}

	template<typename T>
	KPtr<T> Builder()
	{
		return m_kp_Builder;
	}

	template<typename T>
	std::list<KPtr<State_Updater>>* Updater()
	{
		return &m_StateUpdateList;
	}

public:
	template<typename T>
	void Create_Updater()
	{
		T* NewState_Updater = new T();
		NewState_Updater->set_type();
		NewState_Updater->state(this);
		NewState_Updater->window(window());

		m_StateUpdateList.push_back(NewState_Updater);
	}

	template<typename T>
	void Create_Builder()
	{
		if (nullptr != m_kp_Builder)
		{
			m_kp_Builder = nullptr;
		}

		m_kp_Builder = new T();
		m_kp_Builder->state(this);
		m_kp_Builder->window(window());
	}
#pragma endregion

	/********************** State ***********************/
private:
	State();
	~State();

public:
	// state manager;
	StateManager* m_Motherof_State;

	StateManager* statemanager() const
	{
		return m_Motherof_State;
	}

	void statemanager(StateManager* _Value)
	{
		m_Motherof_State = _Value;
	}

public:
	void save(const wchar_t* _Path);
	KPtr<State> load(const wchar_t* _path, 
		State_Builder*(*CBPtr)(const wchar_t*),
		State_Updater*(*CUPtr)(const wchar_t*),
		void(*_ComPtr)(KPtr<TheOne>, const std::string& _Name));



private:
	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator m_S_Map;
	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator m_E_Map;

	std::list<KPtr<TheOne>>::iterator m_S_List;
	std::list<KPtr<TheOne>>::iterator m_E_List;

	std::unordered_map<int, std::list<KPtr<TheOne>>> m_TheOne_Map;

private:
	std::vector<KPtr<TheOne>> All_List();
	bool		m_Build;


public:
	bool&		Is_Build()  { return m_Build; }

public:
	// 에디터에서 쓰므로 통째로 복사해 보여줌
	std::unordered_map<int, std::list<KPtr<TheOne>>>* All_One()
	{
		return &m_TheOne_Map;
	}

private:
	Camera* m_Camera;

public: 
	friend Camera;
	Camera* camera();

	void Build();
	void Start();
	void End();

	// 트랜스폼 기본 추가
	KPtr<TheOne> Create_TheOne(const wchar_t* _Name = L"NewOne", int _Order = 0);
	void Delete_AllofOne();

	bool Is_Name(const wchar_t* _Name);
	size_t Count_Name(const wchar_t* _Name);

private:
	KPtr<TheOne> Create_TheOne(ReadStream& _Stream, void(*_ComPtr)(KPtr<TheOne>, const std::string& _Name), bool _Root = true);

private:
	void Next_Progress();

	void Progress();
	void PrevUpdate();
	void Update();
	void NextUpdate();
	void FinalUpdate();
	void UIRender();
	void DebugUpdate();
	void DebugRender();

	// TheOne의 Componet의 End_Update를 실행한다. 
	void End_Update();


private:
	void Render();
	void Release();


	void insert_One(KPtr<TheOne> _Child);
	bool detach(KPtr<TheOne> _Child);

public:
	RenderManager this_RenderManager;
	Collider2DManager this_Collider2DManager;
};

