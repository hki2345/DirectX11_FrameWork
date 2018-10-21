#pragma once
#include "Begin_Updater.h"

#include <unordered_map>
#include <list>

#include "Mof_KWindow.h"
#include "Mof_State.h"
#include "RenderManager.h"
#include "HCol2DMgr.h"

#include "ReadStream.h"


// c++ 11 이후
// final -->  여기 이후로 상속을 내리지 않는다.

class TheOne;
class StateManager;
class Camera;
class State final : public Begin_Updater, public Mof_KWindow
{
private:
	friend StateManager;
	friend class RenderManager;
	friend TheOne;

#pragma region UPDATEBUILD

public:
	class State_Builder : public Begin_Type, public Mof_State, public Mof_KWindow
	{
	public:
		friend State;


	public:
		virtual void Build_State() = 0;

	protected:
		State_Builder() {}
		~State_Builder() {}
	};

	class State_Updater : public Begin_Type, public Mof_State, public Mof_KWindow
	{
	public:
		friend State;

	public:
		virtual void Start_State() {}
		virtual void Update_State() {}
		virtual void DebugRender() {}

	protected:
		State_Updater() {}
		~State_Updater() {}
	};

private:
	KPtr<State_Builder> m_SBuilder;
	std::list<KPtr<State_Updater>> m_SUpdaterList;
	bool m_bBuild;

public:
	bool IsBuild() {
		return m_bBuild;
	}

private:
	State();
	~State();

public:
	StateManager* m_Mof_StateMgr;

	StateManager* statemanager() {
		return m_Mof_StateMgr;
	}

private:
	void statemanager(StateManager* _SceneMgr) 
	{
		m_Mof_StateMgr = _SceneMgr;
	}


public:
	template<typename T>
	KPtr<T> Updater()
	{
		return m_HSceneUpdater;
	}

	template<typename T>
	KPtr<T> Builder()
	{
		return m_SBuilder;
	}

public:
	template<typename T>
	void CreateUpdater() 
	{
		T* State_Updater = new T();
		State_Updater->Set_Type();
		State_Updater->state(this);
		State_Updater->Window(Window());

		m_SUpdaterList.push_back(State_Updater);
	}

	template<typename T>
	void CreateBuilder()
	{
		if (nullptr != m_SBuilder)
		{
			m_SBuilder = nullptr;
		}

		m_SBuilder = new T();
		m_SBuilder->state(this);
		m_SBuilder->Window(Window());
	}

#pragma endregion

public:
	void Build();
	void Start();

private:
	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator m_StartMapIter;
	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator m_EndMapIter;

	std::list<KPtr<TheOne>>::iterator m_StartListIter;
	std::list<KPtr<TheOne>>::iterator m_EndListIter;
	std::unordered_map<int, std::list<KPtr<TheOne>>> m_ActorMap;

private:
	std::vector<KPtr<TheOne>> AllObjectList();

public:
	std::unordered_map<int, std::list<KPtr<TheOne>>> AllActor() {
		return m_ActorMap;
	}

public:
	friend Camera;

private:
	Camera* m_Camera;

public:
	Camera* Camera();


private:
	void Progress();
	void PrevUpdate();
	void Update();
	void NextUpdate();
	void FinalUpdate();
	void DebugUpdate();

	void NextProgress();
	// TheOne의 Componet의 End_Update를 실행한다. 
	void EndUpdate();

private:
	void Render();
	void DebugRender();
	void Release();

private:
	KPtr<TheOne> Create_One(ReadStream& _Stream, void(*_ComLoadPtr)(KPtr<TheOne>, const std::string& _ComName), bool _Root = true);

public:
	// 트랜스폼 기본 추가
	KPtr<TheOne> Create_One(const wchar_t* _pName = L"HObject", int _Order = 0);

	// 트렌스폼 없음
	KPtr<TheOne> Create_OneNoneTrans(const wchar_t* _pName = L"HObject", int _Order = 0);

private:
	void MoveActor(KPtr<TheOne> _Actor);
	bool EraseActor(KPtr<TheOne> _Actor);

public:
	///////////////////////////// RenderMgr;
	RenderManager RenderMgr;

public:
	///////////////////////////// RenderMgr;
	HCol2DMgr Col2DMgr;

private:
	void OverPushActor(TheOne* _pActor);

public:
	void Save(const wchar_t* _FilePath);

	static KPtr<State> Load(const wchar_t* _FilePath
		, State_Builder*(*CBPtr)(const wchar_t*)
		, State_Updater*(*CUPtr)(const wchar_t*)
		, void(*_ComLoadPtr)(KPtr<TheOne>, const std::string& _ComName));

};

