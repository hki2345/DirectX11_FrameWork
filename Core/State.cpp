#include "State.h"
#include "TheOne.h"
#include "Stl_Assistor.h"
#include "KMacro.h"

#include "Camera.h"
#include "StateManager.h"
#include "WriteStream.h"
#include "ReadStream.h"
#include "ResourceManager.h"
#include "Mouse_Col.h"

#include "Core_Class.h"
#include <atlstr.h>


void State::insert_AbsOne(KPtr<TheOne> _Other)
{
	if (false == Is_Map_Find(m_TheOne_Map, _Other->m_Order))
	{
		m_TheOne_Map.insert(std::unordered_map<int, std::list<KPtr<TheOne>>>::
			value_type(_Other->m_Order,std::list<KPtr<TheOne>>()));
	}

	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator oneIter;

	oneIter = m_TheOne_Map.find(_Other->m_Order);
	if (oneIter == m_TheOne_Map.end())
	{
		return;
	}

	oneIter->second.push_back(_Other);
	_Other->insert_AbsRender(&this_RenderManager);
}



#define MAP_LIST_HELPER(ACTION) \
m_S_Map = m_TheOne_Map.begin(); \
m_E_Map = m_TheOne_Map.end(); \
\
for (; m_S_Map != m_E_Map; ++m_S_Map) \
{ \
	m_S_List = m_S_Map->second.begin(); \
	m_E_List = m_S_Map->second.end(); \
\
	for (; m_S_List != m_E_List; ++m_S_List) \
	{\
		if (false == (*m_S_List)->is_Active()) \
		{ \
			continue;\
		}\
\
		(*m_S_List)->ACTION;\
	}\
}

Camera* State::camera() 
{
	return m_Camera; 
}

State::State() :m_Camera (nullptr), m_Build(false)
{
}


State::~State()
{
}

void State::Progress()
{
	// Mouse_Col::Update();

	// 방어코드 - 한번 더한다 짜피만들어질 때 널이 아닌 거니까.
	if (0 != m_StateUpdateList.size())
	{
		std::list<KPtr<State_Updater>>::iterator S_UpdaterIter = m_StateUpdateList.begin();
		std::list<KPtr<State_Updater>>::iterator E_UpdaterIter = m_StateUpdateList.end();

		for (; S_UpdaterIter != E_UpdaterIter; ++S_UpdaterIter)
		{
			(*S_UpdaterIter)->Update_State();
		}
	}

	PrevUpdate();
	Update();
	NextUpdate();
	FinalUpdate();
	DebugUpdate();
	this_Collider2DManager.progress();
}


void State::Next_Progress()
{
	// 방어코드 - 한번 더한다 짜피만들어질 때 널이 아닌 거니까.// 일단 테스트를 위해 떄려박은 모습 ㅇㅇ
	End_Update();
}

void State::PrevUpdate()
{
	MAP_LIST_HELPER(PrevUpdate());
}

void State::Update()
{
	MAP_LIST_HELPER(Update());
}

void State::NextUpdate()
{
	MAP_LIST_HELPER(NextUpdate());
}


void State::FinalUpdate()
{
	MAP_LIST_HELPER(FinalUpdate());
}

void State::UIRender()
{
	MAP_LIST_HELPER(UIRender());

	if (0 != m_StateUpdateList.size())
	{
		std::list<KPtr<State_Updater>>::iterator S_UpdaterIter = m_StateUpdateList.begin();
		std::list<KPtr<State_Updater>>::iterator E_UpdaterIter = m_StateUpdateList.end();

		for (; S_UpdaterIter != E_UpdaterIter; ++S_UpdaterIter)
		{
			(*S_UpdaterIter)->UI_Render();
		}
	}
}

void State::DebugUpdate()
{
	MAP_LIST_HELPER(DebugUpdate());
}

void State::DebugRender()
{
	MAP_LIST_HELPER(DebugRender());

	if (0 != m_StateUpdateList.size())
	{
		std::list<KPtr<State_Updater>>::iterator S_UpdaterIter = m_StateUpdateList.begin();
		std::list<KPtr<State_Updater>>::iterator E_UpdaterIter = m_StateUpdateList.end();

		for (; S_UpdaterIter != E_UpdaterIter; ++S_UpdaterIter)
		{
			(*S_UpdaterIter)->Debug_Render();
		}
	}
}

void State::End_Update()
{
	MAP_LIST_HELPER(End_Update());
}

void State::Render()
{
	this_RenderManager.Render();
}

void State::Release()
{
	this_RenderManager.Release();
	this_Collider2DManager.release();

	m_S_Map = m_TheOne_Map.begin(); 
	m_E_Map = m_TheOne_Map.end(); 
		
	for (; m_S_Map != m_E_Map; ++m_S_Map) 
	{ 
		m_S_List = m_S_Map->second.begin(); 
		m_E_List = m_S_Map->second.end(); 
			
		for (; m_S_List != m_E_List; ) 
		{
			if (false == (*m_S_List)->is_Death()) 
			{ 
				(*m_S_List)->Release();
				++m_S_List;
				continue; 
			}
					
			m_S_List = m_S_Map->second.erase(m_S_List);;
		}
	}
}


void State::Build()
{
	if (nullptr != m_kp_Builder)
	{
		m_kp_Builder->Build_State();
	}
	m_Build = true;

}

void State::Start()
{
	if (0 != m_StateUpdateList.size())
	{
		std::list<KPtr<State_Updater>>::iterator S_UpdaterIter = m_StateUpdateList.begin();
		std::list<KPtr<State_Updater>>::iterator E_UpdaterIter = m_StateUpdateList.end();

		for (; S_UpdaterIter != E_UpdaterIter; ++S_UpdaterIter)
		{
			Mouse_Col::Init();
			(*S_UpdaterIter)->Start_State();
		}
	}
}

void State::End()
{
	if (0 != m_StateUpdateList.size())
	{
		std::list<KPtr<State_Updater>>::iterator S_UpdaterIter = m_StateUpdateList.begin();
		std::list<KPtr<State_Updater>>::iterator E_UpdaterIter = m_StateUpdateList.end();

		for (; S_UpdaterIter != E_UpdaterIter; ++S_UpdaterIter)
		{
			(*S_UpdaterIter)->End_State();
		}
	}
}


std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator One_Iter;
KPtr<TheOne> State::Create_TheOne
(const wchar_t* _Name /*= L"HObject"*/, int _Order /*= 0*/)
{
	if (false == Is_Map_Find(m_TheOne_Map, _Order))
	{
		m_TheOne_Map.insert
		(std::unordered_map<int, std::list<KPtr<TheOne>>>::
			value_type(_Order, std::list<KPtr<TheOne>>()));
	}
	One_Iter = m_TheOne_Map.find(_Order);
	TheOne* p_NewOne = new TheOne();
	p_NewOne->name(_Name);
	p_NewOne->order(_Order);
	p_NewOne->set_type();
	p_NewOne->state(this);
	p_NewOne->window(window());
	p_NewOne->Add_Component<TransPosition>();
	

	One_Iter->second.push_back(p_NewOne);

	return p_NewOne;
}

void State::Delete_AllofOne()
{

	m_S_Map = m_TheOne_Map.begin();
	m_E_Map = m_TheOne_Map.end();

	for (; m_S_Map != m_E_Map; ++m_S_Map)
	{
		m_S_List = m_S_Map->second.begin();
		m_E_List = m_S_Map->second.end();

		for (; m_S_List != m_E_List; ++m_S_List)
		{
			(*m_S_List)->set_Death();
		}

		m_S_Map->second.clear();
	}
}




void State::insert_One(KPtr<TheOne> _Child)
{
	One_Iter = m_TheOne_Map.find(_Child->order());

	KASSERT(One_Iter == m_TheOne_Map.end());
	if (One_Iter == m_TheOne_Map.end())
	{
		return;
	}

	One_Iter->second.push_back(_Child);
	return;
}

bool State::detach(KPtr<TheOne> _Child)
{
	One_Iter = m_TheOne_Map.find(_Child->order());

	KASSERT(One_Iter == m_TheOne_Map.end());
	if (One_Iter == m_TheOne_Map.end())
	{
		return false;
	}

	std::list<KPtr<TheOne>>::iterator StartIter = One_Iter->second.begin();
	std::list<KPtr<TheOne>>::iterator EndIter = One_Iter->second.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (_Child == *StartIter)
		{
			One_Iter->second.erase(StartIter);
			return true;
		}
	}


	// 굳이 안 되는 거 한 번 더 확인한ㄴ 거
	KASSERT(One_Iter == m_TheOne_Map.end());
	return false;
}



std::vector<KPtr<TheOne>> State::All_List()
{
	std::vector<KPtr<TheOne>> NewVec;

	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator StartMapIter;
	std::unordered_map<int, std::list<KPtr<TheOne>>>::iterator EndMapIter;

	// 리저브를 안 하면 벡터를 쓰는 이유가 없다 ㄷㄷ
	size_t ReserveSize = 0;

	StartMapIter = m_TheOne_Map.begin();
	EndMapIter = m_TheOne_Map.end();
	for (; StartMapIter != EndMapIter; ++StartMapIter)
	{
		ReserveSize += StartMapIter->second.size();
	}

	// 리저브 하는 모습 ㄷㄷ
	NewVec.reserve(ReserveSize);

	std::list<KPtr<TheOne>>::iterator StartListIter;
	std::list<KPtr<TheOne>>::iterator EndListIter;

	StartMapIter = m_TheOne_Map.begin();
	EndMapIter = m_TheOne_Map.end();
	for (; StartMapIter != EndMapIter; ++StartMapIter)
	{
		StartListIter = StartMapIter->second.begin();
		EndListIter = StartMapIter->second.end();
		for (; StartListIter != EndListIter; ++StartListIter)
		{
			NewVec.push_back((*StartListIter));
		}
	}

	return NewVec;
}

void State::save(const wchar_t* _Path)
{
	if (nullptr == PathManager::Find_Path(L"State"))
	{
		KASSERT(true);
	}

	std::wstring Name = PathManager::Find_Path(L"State");
	Name += _Path;

	WriteStream stream = WriteStream(Name.c_str());

	UINT size = name_size() + 1;
	stream.write(&size, sizeof(UINT));
	stream.write_template(name(), sizeof(wchar_t) * size);

	if (nullptr !=  m_kp_Builder)
	{
		std::wstring BuildName = CA2W(m_kp_Builder->typeinfo()->name());
		size = (UINT)BuildName.size() + 1;
		stream.write(&size, sizeof(UINT));
		stream.write_template(BuildName.c_str(), sizeof(wchar_t) * size);
	}
	else
	{
		size = 0;
		stream.write(&size, sizeof(UINT));
	}

	std::list<KPtr<State_Updater>>::iterator S_UpdatIeter = m_StateUpdateList.begin();
	std::list<KPtr<State_Updater>>::iterator E_UpdatIeter = m_StateUpdateList.end();

	size = 0;
	for (; S_UpdatIeter != E_UpdatIeter; ++S_UpdatIeter)
	{
		if (true == (*S_UpdatIeter)->is_debug())
		{
			continue;
		}

		++size;
	}

	stream.write(&size, sizeof(UINT));

	S_UpdatIeter = m_StateUpdateList.begin();
	E_UpdatIeter = m_StateUpdateList.end();

	for (; S_UpdatIeter != E_UpdatIeter; ++S_UpdatIeter)
	{
		if (true == (*S_UpdatIeter)->is_debug())
		{
			continue;
		}

		// 2015.12 시점에서 멀티바이트<->유니코드 문자 변환에 가자아 용이한 것 CA2W
		std::wstring UpdaterName = CA2W((*S_UpdatIeter)->typeinfo()->name());
		size = (UINT)UpdaterName.size() + 1;
		stream.write(&size, sizeof(UINT));
		stream.write_template(UpdaterName.c_str(), sizeof(wchar_t) * size);
	}

	std::vector<KPtr<TheOne>> allOne = All_List();

	if (0 == allOne.size())
	{
		return;
	}

	for (size_t i = 0; i < allOne.size(); i++)
	{
		allOne[i]->save(stream);
	}
}

KPtr<State> State::load(const wchar_t* _path, State_Builder*(*CBPtr)(const wchar_t*), State_Updater*(*CUPtr)(const wchar_t*),
	void(*_ComPtr)(KPtr<TheOne>, const std::string& _Name))
{
	ReadStream NewStream = ReadStream(_path);

	wchar_t Name[1024];

	UINT NewSize;
	NewStream.read(&NewSize, sizeof(UINT));
	NewStream.read((void*)Name, sizeof(wchar_t) * NewSize);

	Name[0] += 1;

	KPtr<State> loadState = Core_Class::Main_StateManager().create_state(Name);

	if (nullptr == loadState)
	{
		return nullptr;
	}

	NewStream.read(&NewSize, sizeof(UINT));

	if (0 != NewSize)
	{
		NewStream.read((void*)Name, sizeof(wchar_t) * NewSize);

		if (nullptr != CBPtr)
		{
			loadState->m_kp_Builder = CBPtr(Name);
		}
	}


	NewStream.read(&NewSize, sizeof(UINT));

	for (size_t i = 0; i < NewSize; i++)
	{
		NewStream.read((void*)Name, sizeof(wchar_t) * NewSize);
		if (nullptr != CUPtr)
		{
			loadState->m_kp_Builder = CBPtr(Name);
		}
	}

	return nullptr;
}

KPtr<TheOne> State::Create_TheOne(ReadStream& _Stream, void(*_ComPtr)(KPtr<TheOne>, const std::string& _Name), bool _Root)
{
	wchar_t Name[1024];

	UINT ReadSize;
	_Stream.read(&ReadSize, sizeof(UINT));

	if (true == _Root)
	{
		if (false == Is_Map_Find(m_TheOne_Map, ReadSize))
		{
			m_TheOne_Map.insert(std::unordered_map<int, std::list<KPtr<TheOne>>>::value_type(ReadSize, std::list<KPtr<TheOne>>()));
		}

		One_Iter = m_TheOne_Map.find(ReadSize);
	}

	TheOne* NewOne = new TheOne();
	NewOne->set_type();
	NewOne->window(window());
	NewOne->state(this);
	NewOne->order(ReadSize);

	_Stream.read(&ReadSize, sizeof(UINT));
	_Stream.read((void*)Name, sizeof(wchar_t) * ReadSize);

	NewOne->name(Name);

	if (true == _Root)
	{
		One_Iter->second.push_back(NewOne);
	}


	_Stream.read(&ReadSize, sizeof(UINT));

	std::string ComName;

	for (size_t i = 0; i < ReadSize; i++)
	{
		char Arr[256];
		unsigned int Size;
		_Stream.read(&Size, sizeof(unsigned int));
		_Stream.read(Arr, sizeof(char) * Size);

		ComName = Arr;

		if (nullptr == _ComPtr)
		{
			continue;
		}
		_ComPtr(NewOne, ComName);
	}

	_Stream.read(&ReadSize, sizeof(UINT));

	for (size_t i = 0; i < ReadSize; i++)
	{
		KPtr<TheOne> NewChild = Create_TheOne(_Stream, _ComPtr, false);
		NewOne->insert_Child(NewChild, true);
	}

	return NewOne;
}

bool State::Is_Name(const wchar_t* _Name) 
{
	m_S_Map = m_TheOne_Map.begin();
	m_E_Map = m_TheOne_Map.end();

	for (; m_S_Map != m_E_Map; ++m_S_Map)
	{
		m_S_List = m_S_Map->second.begin();
		m_E_List = m_S_Map->second.end();

		for (; m_S_List != m_E_List; ++m_S_List)
		{
			std::wstring OneName = (*m_S_List)->name();
			if (OneName == _Name && false == (*m_S_List)->is_Death())
			{
				return true;
			}
		}
	}
	
	return false;
}

size_t State::Count_Name(const wchar_t* _Name)
{
	size_t Count = 0;

	m_S_Map = m_TheOne_Map.begin();
	m_E_Map = m_TheOne_Map.end();

	for (; m_S_Map != m_E_Map; ++m_S_Map)
	{
		m_S_List = m_S_Map->second.begin();
		m_E_List = m_S_Map->second.end();

		for (; m_S_List != m_E_List; ++m_S_List)
		{
			std::wstring OneName = (*m_S_List)->name();
			if (OneName == _Name && false == (*m_S_List)->is_Death())
			{
				++Count;
			}
		}
	}

	return Count;
}