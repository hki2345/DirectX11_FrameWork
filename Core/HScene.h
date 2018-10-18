#pragma once
#include "HUpdaterBase.h"

#include <unordered_map>
#include <list>

#include "HWindowBase.h"
#include "HSceneBase.h"
#include "HRenderMgr.h"
#include "HCol2DMgr.h"

#include "BRStream.h"
// c++ 11 이후

class HActor;
class HSceneMgr;
class HCamera;
class HScene final : public HUpdaterBase, public HWindowBase
{
private:
	friend HSceneMgr;
	friend class HRenderMgr;
	friend HActor;

#pragma region UPDATEBUILD
public:
	class HSceneUpdater : public Begin_Type, public HSceneBase, public HWindowBase
	{
	public:
		friend HScene;

	public:
		virtual void SceneStart() {}
		virtual void SceneUpdate() {}
		virtual void DebugRender() {}

	protected:
		HSceneUpdater() {}
		~HSceneUpdater() {}
	};

	class HSceneBuilder : public Begin_Type, public HSceneBase, public HWindowBase
	{
	public:
		friend HScene;


	public:
		virtual void SceneBuild() = 0;

	protected:
		HSceneBuilder() {}
		~HSceneBuilder() {}
	};

private:
	KPtr<HSceneBuilder> m_HSceneBuilder;
	std::list<KPtr<HSceneUpdater>> m_HSceneUpdaterList;
	bool m_bBuild;

public:
	bool IsBuild() {
		return m_bBuild;
	}

private:
	HScene();
	~HScene();

public:
	HSceneMgr* m_ParentSceneMgr;

	HSceneMgr* SceneMgr() {
		return m_ParentSceneMgr;
	}

private:
	void SceneMgr(HSceneMgr* _SceneMgr) 
	{
		m_ParentSceneMgr = _SceneMgr;
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
		return m_HSceneBuilder;
	}

public:
	template<typename T>
	void CreateUpdater() 
	{
		T* HSceneUpdater = new T();
		HSceneUpdater->Set_Type();
		HSceneUpdater->Scene(this);
		HSceneUpdater->Window(Window());

		m_HSceneUpdaterList.push_back(HSceneUpdater);
	}

	template<typename T>
	void CreateBuilder()
	{
		if (nullptr != m_HSceneBuilder)
		{
			m_HSceneBuilder = nullptr;
		}

		m_HSceneBuilder = new T();
		m_HSceneBuilder->Scene(this);
		m_HSceneBuilder->Window(Window());
	}

#pragma endregion

public:
	void Build();
	void Start();

private:
	std::unordered_map<int, std::list<KPtr<HActor>>>::iterator m_StartMapIter;
	std::unordered_map<int, std::list<KPtr<HActor>>>::iterator m_EndMapIter;

	std::list<KPtr<HActor>>::iterator m_StartListIter;
	std::list<KPtr<HActor>>::iterator m_EndListIter;
	std::unordered_map<int, std::list<KPtr<HActor>>> m_ActorMap;

private:
	std::vector<KPtr<HActor>> AllObjectList();

public:
	std::unordered_map<int, std::list<KPtr<HActor>>> AllActor() {
		return m_ActorMap;
	}

public:
	friend HCamera;

private:
	HCamera* m_Camera;

public:
	HCamera* Camera();


private:
	void Progress();
	void PrevUpdate();
	void Update();
	void NextUpdate();
	void FinalUpdate();
	void DebugUpdate();

	void AfterProgress();
	void EndUpdate();

private:
	void Render();
	void DebugRender();
	void Release();

private:
	KPtr<HActor> CreateActor(BRStream& _Stream, void(*_ComLoadPtr)(KPtr<HActor>, const std::string& _ComName), bool _Root = true);

public:
	// 트랜스폼이 기본 추가 됩니다.
	KPtr<HActor> CreateActor(const wchar_t* _pName = L"HObject", int _Order = 0);

	// 트랜스폼이 없는 액터를 리턴합니다.
	KPtr<HActor> CreateNoneTransActor(const wchar_t* _pName = L"HObject", int _Order = 0);

private:
	void MoveActor(KPtr<HActor> _Actor);
	bool EraseActor(KPtr<HActor> _Actor);

public:
	///////////////////////////// RenderMgr;
	HRenderMgr RenderMgr;

public:
	///////////////////////////// RenderMgr;
	HCol2DMgr Col2DMgr;

private:
	void OverPushActor(HActor* _pActor);

public:
	void Save(const wchar_t* _FilePath);

	static KPtr<HScene> Load(const wchar_t* _FilePath
		, HSceneBuilder*(*CBPtr)(const wchar_t*)
		, HSceneUpdater*(*CUPtr)(const wchar_t*)
		, void(*_ComLoadPtr)(KPtr<HActor>, const std::string& _ComName));

};

