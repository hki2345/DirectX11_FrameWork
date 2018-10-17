#pragma once
#include <unordered_map>
#include <string>
#include "HScene.h"

class HActor;
class HWindow;
class HSceneMgr : public HWindowBase
{
public:
	friend HWindow;
	friend HActor;

private:
	std::list<HPTR<HActor>> m_OverActorList;

public:
	void OverCheck(HScene* _NewScene);

public:
	bool OverFind(HPTR<HActor> _Actor);

private:
	std::unordered_map<std::wstring, HPTR<HScene>> m_SceneMap;
	HPTR<HScene> m_CurScene;
	HPTR<HScene> m_NextScene;

public:
	HPTR<HScene> CurScene() {
		return m_CurScene;
	}

	HPTR<HScene> NextScene() {
		return m_NextScene;
	}

public:
	template<typename Builder, typename Updater>
	HPTR<HScene> CreateScene(const wchar_t* _Name, bool _bBuild = true)
	{
		if (nullptr != FindScene(_Name))
		{
			return nullptr;
		}

		HScene* pNewScene = new HScene();
		pNewScene->SceneMgr(this);
		pNewScene->Name(_Name);
		pNewScene->Window(Window());
		pNewScene->TypeSetting();

		OverCheck(pNewScene);

		pNewScene->CreateBuilder<Builder>();
		pNewScene->CreateUpdater<Updater>();

		if (true == _bBuild)
		{
			pNewScene->Build();
		}

		m_SceneMap.insert(std::unordered_map<std::wstring, HPTR<HScene>>::value_type(_Name,pNewScene));
		return pNewScene;
	}

	HPTR<HScene> CreateScene(const wchar_t* _Name);

	HPTR<HScene> FindScene(const wchar_t* _Name);
	void ChangeScene(const wchar_t* _Name);

private:
	void Progress();
	void Render();
	void AfterProgress();
	void Release();

public:
	HSceneMgr(HWindow* _Window);
	~HSceneMgr();
};

