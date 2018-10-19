#include "HScene.h"
#include "HActor.h"
#include "Stl_AID.h"
#include "HTrans.h"
#include "KMacro.h"
#include "BWStream.h"
#include "BRStream.h"
#include "HSceneMgr.h"
#include "Core_Class.h"
#include <atlstr.h>

HCamera* HScene::Camera() 
{
	return m_Camera;
}

HScene::HScene() : m_Camera(nullptr), m_bBuild(false)
{
}


HScene::~HScene()
{
	int a = 0;
}


void HScene::Progress()
{
	if (0 != m_HSceneUpdaterList.size())	
	{	
		std::list<KPtr<HSceneUpdater>>::iterator StartUpdaterIter = m_HSceneUpdaterList.begin();
		std::list<KPtr<HSceneUpdater>>::iterator EndUpdaterIter = m_HSceneUpdaterList.end();

		for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
		{
			(*StartUpdaterIter)->SceneUpdate();
		}
	}

	PrevUpdate();
	Update();
	NextUpdate();
	FinalUpdate();
	DebugUpdate();
	Col2DMgr.Progress();
}

void HScene::PrevUpdate() {
	m_StartMapIter = m_ActorMap.begin();
	m_EndMapIter = m_ActorMap.end();

	for (; m_StartMapIter != m_EndMapIter; ++m_StartMapIter)
	{
		m_StartListIter = m_StartMapIter->second.begin();
		m_EndListIter = m_StartMapIter->second.end();

		for (; m_StartListIter != m_EndListIter; ++m_StartListIter)
		{
			if (false == (*m_StartListIter)->Is_Active())
			{
				continue;
			}

			(*m_StartListIter)->PrevUpdate();
		}
	}
}
void HScene::Update() {
	m_StartMapIter = m_ActorMap.begin();
	m_EndMapIter = m_ActorMap.end();

	for (; m_StartMapIter != m_EndMapIter; ++m_StartMapIter)
	{
		m_StartListIter = m_StartMapIter->second.begin();
		m_EndListIter = m_StartMapIter->second.end();

		for (; m_StartListIter != m_EndListIter; ++m_StartListIter)
		{
			if (false == (*m_StartListIter)->Is_Active())
			{
				continue;
			}

			(*m_StartListIter)->Update();
		}
	}
}
void HScene::NextUpdate() {
	m_StartMapIter = m_ActorMap.begin();
	m_EndMapIter = m_ActorMap.end();

	for (; m_StartMapIter != m_EndMapIter; ++m_StartMapIter)
	{
		m_StartListIter = m_StartMapIter->second.begin();
		m_EndListIter = m_StartMapIter->second.end();

		for (; m_StartListIter != m_EndListIter; ++m_StartListIter)
		{
			if (false == (*m_StartListIter)->Is_Active())
			{
				continue;
			}

			(*m_StartListIter)->NextUpdate();
		}
	}
}


void HScene::FinalUpdate() {
	m_StartMapIter = m_ActorMap.begin();
	m_EndMapIter = m_ActorMap.end();

	for (; m_StartMapIter != m_EndMapIter; ++m_StartMapIter)
	{
		m_StartListIter = m_StartMapIter->second.begin();
		m_EndListIter = m_StartMapIter->second.end();

		for (; m_StartListIter != m_EndListIter; ++m_StartListIter)
		{
			if (false == (*m_StartListIter)->Is_Active())
			{
				continue;
			}

			(*m_StartListIter)->FinalUpdate();
		}
	}
}

void HScene::DebugUpdate() {
	m_StartMapIter = m_ActorMap.begin();
	m_EndMapIter = m_ActorMap.end();

	for (; m_StartMapIter != m_EndMapIter; ++m_StartMapIter)
	{
		m_StartListIter = m_StartMapIter->second.begin();
		m_EndListIter = m_StartMapIter->second.end();

		for (; m_StartListIter != m_EndListIter; ++m_StartListIter)
		{
			if (false == (*m_StartListIter)->Is_Active())
			{
				continue;
			}

			(*m_StartListIter)->DebugUpdate();
		}
	}
}

void HScene::Render() 
{
	RenderMgr.Render();
}

void HScene::DebugRender() {
	m_StartMapIter = m_ActorMap.begin();
	m_EndMapIter = m_ActorMap.end();

	for (; m_StartMapIter != m_EndMapIter; ++m_StartMapIter)
	{
		m_StartListIter = m_StartMapIter->second.begin();
		m_EndListIter = m_StartMapIter->second.end();

		for (; m_StartListIter != m_EndListIter; ++m_StartListIter)
		{
			if (false == (*m_StartListIter)->Is_Active())
			{
				continue;
			}

			(*m_StartListIter)->DebugRender();
		}
	}

	if (0 != m_HSceneUpdaterList.size())
	{
		std::list<KPtr<HSceneUpdater>>::iterator StartUpdaterIter = m_HSceneUpdaterList.begin();
		std::list<KPtr<HSceneUpdater>>::iterator EndUpdaterIter = m_HSceneUpdaterList.end();

		for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
		{
			(*StartUpdaterIter)->DebugRender();
		}
	}
}

void HScene::Release() 
{
	RenderMgr.Release();
	Col2DMgr.Release();

	m_StartMapIter = m_ActorMap.begin();
	m_EndMapIter = m_ActorMap.end();

	for (; m_StartMapIter != m_EndMapIter; ++m_StartMapIter)
	{
		m_StartListIter = m_StartMapIter->second.begin();
		m_EndListIter = m_StartMapIter->second.end();

		for (; m_StartListIter != m_EndListIter; )
		{
			if (false == (*m_StartListIter)->Is_Death())
			{
				(*m_StartListIter)->Release();
				++m_StartListIter;
				continue;
			}

			m_StartListIter = m_StartMapIter->second.erase(m_StartListIter);
		}
	}
}

void HScene::Build() 
{
	if (nullptr != m_HSceneBuilder)
	{
		m_HSceneBuilder->SceneBuild();
	}


	m_bBuild = true;

} 

void HScene::Start() 
{
	if (0 != m_HSceneUpdaterList.size())
	{
		std::list<KPtr<HSceneUpdater>>::iterator StartUpdaterIter = m_HSceneUpdaterList.begin();
		std::list<KPtr<HSceneUpdater>>::iterator EndUpdaterIter = m_HSceneUpdaterList.end();

		for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
		{
			(*StartUpdaterIter)->SceneStart();
		}
	}
}

std::unordered_map<int, std::list<KPtr<HActor>>>::iterator ActorIter;
KPtr<HActor> HScene::CreateActor(const wchar_t* _pName /*= L"HObject"*/, int _Order /*= 0*/)
{
	if (false == Is_MapFind(m_ActorMap, _Order)) 
	{
		m_ActorMap.insert(std::unordered_map<int, std::list<KPtr<HActor>>>::value_type(_Order, std::list<KPtr<HActor>>()));
	}
	ActorIter = m_ActorMap.find(_Order);
	HActor* pNewActor = new HActor();
	pNewActor->Name(_pName);
	pNewActor->Order(_Order);
	pNewActor->Set_Type();
	pNewActor->Window(Window());
	pNewActor->Scene(this);
	pNewActor->AddCom<HTrans>();

	ActorIter->second.push_back(pNewActor);

	return pNewActor;
}

KPtr<HActor> HScene::CreateNoneTransActor(const wchar_t* _pName /*= L"HObject"*/, int _Order /*= 0*/)
{
	if (false == Is_MapFind(m_ActorMap, _Order))
	{
		m_ActorMap.insert(std::unordered_map<int, std::list<KPtr<HActor>>>::value_type(_Order, std::list<KPtr<HActor>>()));
	}
	ActorIter = m_ActorMap.find(_Order);
	HActor* pNewActor = new HActor();
	pNewActor->Name(_pName);
	pNewActor->Order(_Order);
	pNewActor->Set_Type();
	pNewActor->Window(Window());
	pNewActor->Scene(this);

	ActorIter->second.push_back(pNewActor);

	return pNewActor;
}


void HScene::MoveActor(KPtr<HActor> _Actor)
{
	ActorIter = m_ActorMap.find(_Actor->Order());

	KASSERT(ActorIter == m_ActorMap.end());
	if (ActorIter == m_ActorMap.end())
	{
		return;
	}

	ActorIter->second.push_back(_Actor);

	return;

}

bool HScene::EraseActor(KPtr<HActor> _Actor)
{
	ActorIter = m_ActorMap.find(_Actor->Order());

	KASSERT(ActorIter == m_ActorMap.end());
	if (ActorIter == m_ActorMap.end()) {
		return false;
	}

	std::list<KPtr<HActor>>::iterator StartListIter = ActorIter->second.begin();
	std::list<KPtr<HActor>>::iterator EndListIter = ActorIter->second.end();

	for (; StartListIter != EndListIter; StartListIter++)
	{
		if (_Actor == *StartListIter)
		{
			ActorIter->second.erase(StartListIter);
			return true;
		}
	}

	KASSERT(ActorIter == m_ActorMap.end());
	return false;
}

void HScene::AfterProgress()
{
	EndUpdate();
}

void HScene::EndUpdate() {
	m_StartMapIter = m_ActorMap.begin();
	m_EndMapIter = m_ActorMap.end();

	for (; m_StartMapIter != m_EndMapIter; ++m_StartMapIter)
	{
		m_StartListIter = m_StartMapIter->second.begin();
		m_EndListIter = m_StartMapIter->second.end();

		for (; m_StartListIter != m_EndListIter; ++m_StartListIter)
		{
			if (false == (*m_StartListIter)->Is_Active())
			{
				continue;
			}

			(*m_StartListIter)->EndUpdate();
		}
	}
}

void HScene::OverPushActor(HActor* _pActor) 
{
	if (false == Is_MapFind(m_ActorMap, _pActor->m_Order))
	{
		m_ActorMap.insert(std::unordered_map<int, std::list<KPtr<HActor>>>::value_type(_pActor->m_Order, std::list<KPtr<HActor>>()));
	}

	ActorIter = m_ActorMap.find(_pActor->m_Order);

	if (ActorIter == m_ActorMap.end())
	{
		return;
	}
	// ���͸� �ִ°� ó���� �Ǿ���.
	ActorIter->second.push_back(_pActor);

	// �������� �� �־��ش�.
	_pActor->OverPushRender(&RenderMgr);
	_pActor->OverPushCol2D(&Col2DMgr);
}

std::vector<KPtr<HActor>> HScene::AllObjectList()
{
	std::vector<KPtr<HActor>> ReturnVector;

	std::unordered_map<int, std::list<KPtr<HActor>>>::iterator StartMapIter;
	std::unordered_map<int, std::list<KPtr<HActor>>>::iterator EndMapIter;

	size_t ReserveSize = 0;

	StartMapIter = m_ActorMap.begin();
	EndMapIter = m_ActorMap.end();
	for (; StartMapIter != EndMapIter; ++StartMapIter)
	{
		ReserveSize += StartMapIter->second.size();
	}

	ReturnVector.reserve(ReserveSize);

	std::list<KPtr<HActor>>::iterator StartListIter;
	std::list<KPtr<HActor>>::iterator EndListIter;

	StartMapIter = m_ActorMap.begin();
	EndMapIter = m_ActorMap.end();
	for (; StartMapIter != EndMapIter; ++StartMapIter)
	{
		StartListIter = StartMapIter->second.begin();
		EndListIter = StartMapIter->second.end();
		for (; StartListIter != EndListIter; ++StartListIter)
		{
			ReturnVector.push_back((*StartListIter));
		}
	}

	return ReturnVector;
}

void HScene::Save(const wchar_t* _FilePath) 
{
	BWStream SaveStream = BWStream(_FilePath);

	UINT Size = NameSize() + 1;
	SaveStream.Write(&Size, sizeof(UINT));
	SaveStream.WriteT(Name(), sizeof(wchar_t) * Size);

	if (nullptr != m_HSceneBuilder)
	{
		std::wstring BuilderName = CA2W(m_HSceneBuilder->TypeInfo()->name());
		Size = (UINT)BuilderName.size() + 1;
		SaveStream.Write(&Size, sizeof(UINT));
		SaveStream.WriteT(BuilderName.c_str(), sizeof(wchar_t) * Size);
	}
	else {
		Size = 0;
		SaveStream.Write(&Size, sizeof(UINT));
	}

	// ���������� �� ���� ����� ������ ���´�.
	std::list<KPtr<HSceneUpdater>>::iterator StartUpdaterIter = m_HSceneUpdaterList.begin();
	std::list<KPtr<HSceneUpdater>>::iterator EndUpdaterIter = m_HSceneUpdaterList.end();
	Size = 0;
	for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
	{
		if (true == (*StartUpdaterIter)->IsDebug())
		{
			continue;
		}

		++Size;
	}

	SaveStream.Write(&Size, sizeof(UINT));

	// ���� ���������� �̸����� ������ ���´�.
	StartUpdaterIter = m_HSceneUpdaterList.begin();
	EndUpdaterIter = m_HSceneUpdaterList.end();
	for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
	{
		if (true == (*StartUpdaterIter)->IsDebug())
		{
			continue;
		}

		std::wstring UpdaterName = CA2W((*StartUpdaterIter)->TypeInfo()->name());
		Size = (UINT)UpdaterName.size() + 1;
		SaveStream.Write(&Size, sizeof(UINT));
		SaveStream.WriteT(UpdaterName.c_str(), sizeof(wchar_t) * Size);
	}

	std::vector<KPtr<HActor>> AllActor = AllObjectList();

	if (0 == AllActor.size())
	{
		return;
	}

	for (size_t i = 0; i < AllActor.size(); i++)
	{
		AllActor[i]->Save(SaveStream);
	}

	int a = 0;
}

KPtr<HScene> HScene::Load(const wchar_t* _FilePath, HSceneBuilder*(*CBPtr)(const wchar_t*), HSceneUpdater*(*CUPtr)(const wchar_t*), void(*_ComLoadPtr)(KPtr<HActor>, const std::string& _ComName))
{
	BRStream LoadStream = BRStream(_FilePath);

	wchar_t Name[1024];

	UINT ReadSize;
	LoadStream.Read(&ReadSize, sizeof(UINT));
	LoadStream.Read((void*)Name, sizeof(wchar_t) * ReadSize);

	Name[0] += 1;

	KPtr<HScene> LoadScene = Core_Class::MainSceneMgr().CreateScene(Name);

	if (nullptr == LoadScene)
	{
		return nullptr;
	}

	// ���� ������ �Ѵ�.

	LoadStream.Read(&ReadSize, sizeof(UINT));

	if (0 != ReadSize)
	{
		LoadStream.Read((void*)Name, sizeof(wchar_t) * ReadSize);
		if (nullptr != CBPtr)
		{
			LoadScene->m_HSceneBuilder = CBPtr(Name);
		}
	}

	LoadStream.Read(&ReadSize, sizeof(UINT));

	for (size_t i = 0; i < ReadSize; i++)
	{
		LoadStream.Read((void*)Name, sizeof(wchar_t) * ReadSize);
		if (nullptr != CUPtr)
		{
			LoadScene->m_HSceneUpdaterList.push_back(CUPtr(Name));
		}
	}

	return nullptr;
}

KPtr<HActor> HScene::CreateActor(BRStream& _Stream, void(*_ComLoadPtr)(KPtr<HActor>, const std::string& _ComName), bool _Root)
{
	wchar_t Name[1024];

	UINT ReadSize;
	_Stream.Read(&ReadSize, sizeof(UINT));

	if (true == _Root)
	{
		if (false == Is_MapFind(m_ActorMap, ReadSize))
		{
			m_ActorMap.insert(std::unordered_map<int, std::list<KPtr<HActor>>>::value_type(ReadSize, std::list<KPtr<HActor>>()));
		}
		ActorIter = m_ActorMap.find(ReadSize);
	}

	HActor* pNewActor = new HActor();
	pNewActor->Set_Type();
	pNewActor->Window(Window());
	pNewActor->Scene(this);
	pNewActor->Order(ReadSize);

	_Stream.Read(&ReadSize, sizeof(UINT));
	_Stream.Read((void*)Name, sizeof(wchar_t) * ReadSize);

	pNewActor->Name(Name);

	if (true == _Root)
	{
		ActorIter->second.push_back(pNewActor);
	}

	_Stream.Read(&ReadSize, sizeof(UINT));

	std::string ComName;

	for (size_t i = 0; i < ReadSize; i++)
	{
		char Arr[256];
		unsigned int Size;
		_Stream.Read(&Size, sizeof(unsigned int));
		_Stream.Read(Arr, sizeof(char) * Size);

		ComName = Arr;

		if (nullptr == _ComLoadPtr)
		{
			continue;
		}
		_ComLoadPtr(pNewActor, ComName);
	}

	_Stream.Read(&ReadSize, sizeof(UINT));

	for (size_t i = 0; i < ReadSize; i++)
	{
		KPtr<HActor> ChildActor = CreateActor(_Stream, _ComLoadPtr, false);
		pNewActor->AddChild(ChildActor, true);
	}

	return pNewActor;
}