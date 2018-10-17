#include "HSceneMgr.h"
#include "StlHelperFunc.h"
#include "HMACRO.h"
#include "HWindow.h"
#include "GameDebug.h"

bool HSceneMgr::OverFind(HPTR<HActor> _Actor)
{
	std::list<HPTR<HActor>>::iterator StartOverIter = m_OverActorList.begin();
	std::list<HPTR<HActor>>::iterator EndOverIter = m_OverActorList.end();

	for (; StartOverIter != EndOverIter; ++StartOverIter)
	{
		if ((*StartOverIter) == _Actor)
		{
			return true;
		}
	}

	return false;
}

void HSceneMgr::OverCheck(HScene* _NewScene) 
{
	std::list<HPTR<HActor>>::iterator StartIter = m_OverActorList.begin();
	std::list<HPTR<HActor>>::iterator EndIter = m_OverActorList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (OT_ALLOVER != (*StartIter)->OverType())
		{
			continue;
		}

		_NewScene->OverPushActor(*StartIter);
	}

}


HSceneMgr::HSceneMgr(HWindow* _Window)
{
	Window(_Window);
}

HSceneMgr::~HSceneMgr()
{
}

void HSceneMgr::ChangeScene(const wchar_t* _Name) 
{
	if (nullptr == _Name)
	{
		m_NextScene = nullptr;
		return;
	}

	m_NextScene = FindScene(_Name);
	TASSERT(nullptr == m_NextScene);
}

HPTR<HScene> HSceneMgr::FindScene(const wchar_t* _Name) 
{
	return MapFind<HPTR<HScene>>(m_SceneMap, _Name);
}

void HSceneMgr::Progress() 
{
	if (nullptr != m_NextScene)
	{
		m_CurScene = m_NextScene;
		m_NextScene = nullptr;

		if (false == m_CurScene->IsBuild())
		{
			m_CurScene->Build();
		}

		m_CurScene->Start();
	}

	if (nullptr != m_CurScene && true == m_CurScene->IsUpdate())
	{
		m_CurScene->Progress();
	}

}

void HSceneMgr::Render()
{
	if (nullptr != m_CurScene && true == m_CurScene->IsUpdate())
	{
		m_CurScene->Render();
	}
}


void HSceneMgr::AfterProgress()
{
	if (nullptr != m_CurScene && true == m_CurScene->IsUpdate())
	{
		m_CurScene->AfterProgress();
	}
}

void HSceneMgr::Release() {
	if (nullptr != m_CurScene && true == m_CurScene->IsUpdate())
	{
		m_CurScene->Release();
	}
}

HPTR<HScene> HSceneMgr::CreateScene(const wchar_t* _Name) 
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

	m_SceneMap.insert(std::unordered_map<std::wstring, HPTR<HScene>>::value_type(_Name, pNewScene));
	return pNewScene;
}