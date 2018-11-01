#include "State.h"
#include "TheOne.h"
#include "Stl_AID.h"
#include "TransPosition.h"
#include "KMacro.h"
#include "WriteStream.h"
#include "ReadStream.h"
#include "StateManager.h"
#include "Core_Class.h"
#include <atlstr.h>

Camera* State::Camera() 
{
	return m_Camera;
}

State::State() : m_Camera(nullptr), m_bBuild(false)
{
}


State::~State()
{
	int a = 0;
}


void State::Progress()
{
	if (0 != m_SUpdaterList.size())	
	{	
		std::list<KPtr<State_Updater>>::iterator StartUpdaterIter = m_SUpdaterList.begin();
		std::list<KPtr<State_Updater>>::iterator EndUpdaterIter = m_SUpdaterList.end();

		for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
		{
			(*StartUpdaterIter)->Update_State();
		}
	}

	PrevUpdate();
	Update();
	NextUpdate();
	FinalUpdate();
	DebugUpdate();
	Col2DMgr.Progress();
}

void State::PrevUpdate() {
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
void State::Update() {
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
void State::NextUpdate() {
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


void State::FinalUpdate() {
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

void State::DebugUpdate() {
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

void State::Render() 
{
	RenderMgr.Render();
}

void State::DebugRender() {
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

	if (0 != m_SUpdaterList.size())
	{
		std::list<KPtr<State_Updater>>::iterator StartUpdaterIter = m_SUpdaterList.begin();
		std::list<KPtr<State_Updater>>::iterator EndUpdaterIter = m_SUpdaterList.end();

		for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
		{
			(*StartUpdaterIter)->DebugRender();
		}
	}
}

void State::Release() 
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

void State::Build() 
{
	if (nullptr != m_SBuilder)
	{
		m_SBuilder->Build_State();
	}


	m_bBuild = true;

} 

void State::Start() 
{
	if (0 != m_SUpdaterList.size())
	{
		std::list<KPtr<State_Updater>>::iterator StartUpdaterIter = m_SUpdaterList.begin();
		std::list<KPtr<State_Updater>>::iterator EndUpdaterIter = m_SUpdaterList.end();

		for (; StartUpdaterIter != EndUpdaterIter; ++StartUpdaterIter)
		{
			(*StartUpdaterIter)->Start_State();
		}
	}
}

std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator ActorIter;
KPtr<TheOne> State::Create_One(const wchar_t* _pName /*= L"HObject"*/, int _Order /*= 0*/)
{
	if (false == Is_MapFind(m_ActorMap, _Order)) 
	{
		m_ActorMap.insert(std::unordered_map<int, std::list<KPtr<TheOne>>>::value_type(_Order, std::list<KPtr<TheOne>>()));
	}
	ActorIter = m_ActorMap.find(_Order);
	TheOne* pNewActor = new TheOne();
	pNewActor->Name(_pName);
	pNewActor->order(_Order);
	pNewActor->Set_Type();
	pNewActor->kwindow(kwindow());
	pNewActor->state(this);
	pNewActor->Add_Component<TransPosition>();

	ActorIter->second.push_back(pNewActor);

	return pNewActor;
}

KPtr<TheOne> State::Create_OneNoneTrans(const wchar_t* _pName /*= L"HObject"*/, int _Order /*= 0*/)
{
	if (false == Is_MapFind(m_ActorMap, _Order))
	{
		m_ActorMap.insert(std::unordered_map<int, std::list<KPtr<TheOne>>>::value_type(_Order, std::list<KPtr<TheOne>>()));
	}
	ActorIter = m_ActorMap.find(_Order);
	TheOne* pNewActor = new TheOne();
	pNewActor->Name(_pName);
	pNewActor->order(_Order);
	pNewActor->Set_Type();
	pNewActor->kwindow(kwindow());
	pNewActor->state(this);

	ActorIter->second.push_back(pNewActor);

	return pNewActor;
}


void State::MoveActor(KPtr<TheOne> _Actor)
{
	ActorIter = m_ActorMap.find(_Actor->order());

	KASSERT(ActorIter == m_ActorMap.end());
	if (ActorIter == m_ActorMap.end())
	{
		return;
	}

	ActorIter->second.push_back(_Actor);

	return;

}

bool State::EraseActor(KPtr<TheOne> _Actor)
{
	ActorIter = m_ActorMap.find(_Actor->order());

	KASSERT(ActorIter == m_ActorMap.end());
	if (ActorIter == m_ActorMap.end()) {
		return false;
	}

	std::list<KPtr<TheOne>>::iterator StartListIter = ActorIter->second.begin();
	std::list<KPtr<TheOne>>::iterator EndListIter = ActorIter->second.end();

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

void State::NextProgress()
{
	EndUpdate();
}

void State::EndUpdate() {
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

void State::OverPushActor(TheOne* _pActor) 
{
	if (false == Is_MapFind(m_ActorMap, _pActor->m_Order))
	{
		m_ActorMap.insert(std::unordered_map<int, std::list<KPtr<TheOne>>>::value_type(_pActor->m_Order, std::list<KPtr<TheOne>>()));
	}

	ActorIter = m_ActorMap.find(_pActor->m_Order);

	if (ActorIter == m_ActorMap.end())
	{
		return;
	}
	// 액터를 넣는건 처리가 되었다.
	ActorIter->second.push_back(_pActor);

	// 랜더러를 다 넣어준다.
	_pActor->Insert_ABSRender(&RenderMgr);
	_pActor->Insert_ABSCol(&Col2DMgr);
}

std::vector<KPtr<TheOne>> State::AllObjectList()
{
	std::vector<KPtr<TheOne>> ReturnVector;

	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator StartMapIter;
	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator EndMapIter;

	size_t ReserveSize = 0;

	StartMapIter = m_ActorMap.begin();
	EndMapIter = m_ActorMap.end();
	for (; StartMapIter != EndMapIter; ++StartMapIter)
	{
		ReserveSize += StartMapIter->second.size();
	}

	ReturnVector.reserve(ReserveSize);

	std::list<KPtr<TheOne>>::iterator StartListIter;
	std::list<KPtr<TheOne>>::iterator EndListIter;

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

KPtr<TheOne> State::Create_One(ReadStream& _Stream, void(*_ComLoadPtr)(KPtr<TheOne>, const std::string& _ComName), bool _Root)
{
	wchar_t Name[1024];

	UINT ReadSize;
	_Stream.Read(&ReadSize, sizeof(UINT));

	if (true == _Root)
	{
		if (false == Is_MapFind(m_ActorMap, ReadSize))
		{
			m_ActorMap.insert(std::unordered_map<int, std::list<KPtr<TheOne>>>::value_type(ReadSize, std::list<KPtr<TheOne>>()));
		}
		ActorIter = m_ActorMap.find(ReadSize);
	}

	TheOne* pNewActor = new TheOne();
	pNewActor->Set_Type();
	pNewActor->kwindow(kwindow());
	pNewActor->state(this);
	pNewActor->order(ReadSize);

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
		KPtr<TheOne> ChildActor = Create_One(_Stream, _ComLoadPtr, false);
		pNewActor->Add_Child(ChildActor, true);
	}

	return pNewActor;
}