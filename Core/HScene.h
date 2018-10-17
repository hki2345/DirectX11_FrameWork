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
	class HSceneUpdater : public HTypeBase, public HSceneBase, public HWindowBase
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

	class HSceneBuilder : public HTypeBase, public HSceneBase, public HWindowBase
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
	HPTR<HSceneBuilder> m_HSceneBuilder;
	std::list<HPTR<HSceneUpdater>> m_HSceneUpdaterList;
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
	HPTR<T> Updater()
	{
		return m_HSceneUpdater;
	}

	template<typename T>
	HPTR<T> Builder()
	{
		return m_HSceneBuilder;
	}

public:
	template<typename T>
	void CreateUpdater() 
	{
		T* HSceneUpdater = new T();
		HSceneUpdater->TypeSetting();
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
	std::unordered_map<int, std::list<HPTR<HActor>>>::iterator m_StartMapIter;
	std::unordered_map<int, std::list<HPTR<HActor>>>::iterator m_EndMapIter;

	std::list<HPTR<HActor>>::iterator m_StartListIter;
	std::list<HPTR<HActor>>::iterator m_EndListIter;
	std::unordered_map<int, std::list<HPTR<HActor>>> m_ActorMap;

private:
	std::vector<HPTR<HActor>> AllObjectList();

public:
	std::unordered_map<int, std::list<HPTR<HActor>>> AllActor() {
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
	HPTR<HActor> CreateActor(BRStream& _Stream, void(*_ComLoadPtr)(HPTR<HActor>, const std::string& _ComName), bool _Root = true);

public:
	// 트랜스폼이 기본 추가 됩니다.
	HPTR<HActor> CreateActor(const wchar_t* _pName = L"HObject", int _Order = 0);

	// 트랜스폼이 없는 액터를 리턴합니다.
	HPTR<HActor> CreateNoneTransActor(const wchar_t* _pName = L"HObject", int _Order = 0);

private:
	void MoveActor(HPTR<HActor> _Actor);
	bool EraseActor(HPTR<HActor> _Actor);

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

	static HPTR<HScene> Load(const wchar_t* _FilePath
		, HSceneBuilder*(*CBPtr)(const wchar_t*)
		, HSceneUpdater*(*CUPtr)(const wchar_t*)
		, void(*_ComLoadPtr)(HPTR<HActor>, const std::string& _ComName));

};

