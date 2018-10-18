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
	std::list<KPtr<HActor>> m_OverActorList;

public:
	void OverCheck(HScene* _NewScene);

public:
	bool OverFind(KPtr<HActor> _Actor);

private:
	std::unordered_map<std::wstring, KPtr<HScene>> m_SceneMap;
	KPtr<HScene> m_CurScene;
	KPtr<HScene> m_NextScene;

public:
	KPtr<HScene> CurScene() {
		return m_CurScene;
	}

	KPtr<HScene> NextScene() {
		return m_NextScene;
	}

public:
	template<typename Builder, typename Updater>
	KPtr<HScene> CreateScene(const wchar_t* _Name, bool _bBuild = true)
	{
		if (nullptr != FindScene(_Name))
		{
			return nullptr;
		}

		HScene* pNewScene = new HScene();
		pNewScene->SceneMgr(this);
		pNewScene->Name(_Name);
		pNewScene->Window(Window());
		pNewScene->Set_Type();

		OverCheck(pNewScene);

		pNewScene->CreateBuilder<Builder>();
		pNewScene->CreateUpdater<Updater>();

		if (true == _bBuild)
		{
			pNewScene->Build();
		}

		m_SceneMap.insert(std::unordered_map<std::wstring, KPtr<HScene>>::value_type(_Name,pNewScene));
		return pNewScene;
	}

	KPtr<HScene> CreateScene(const wchar_t* _Name);

	KPtr<HScene> FindScene(const wchar_t* _Name);
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

