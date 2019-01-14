#include "SC2_Force.h"
#include "Force_Unit.h"


#include <WriteStream.h>
#include <ReadStream.h>
#include <Core_Class.h>
#include <Renderer_Terrain.h>


#include "Controll_AI.h"
#include "Controll_User.h"


SC2_Force::SC2_Force(const wchar_t* _Name, const KColor& _Color)
{
	name(_Name);
	memcpy_s(m_Force.Name, NAMENUM, _Name, NAMENUM);
	this->m_Force.Color = _Color;
}


SC2_Force::~SC2_Force()
{
} 


KPtr<Force_Unit> SC2_Force::Create_Unit(const wchar_t* _Name, KPtr<Renderer_Terrain> _Ter)
{
	KPtr<State> TabScene = Core_Class::MainScene();
	KPtr<TheOne> TOne = TabScene->Create_One();

	TOne->Trans()->pos_local(KVector::Zero);
	TOne->Trans()->scale_local(KVector(1.f, 1.f, 1.f));
	KPtr<Force_Unit> TT = TOne->Add_Component<Force_Unit>(_Name, _Ter, false);
	TT->force(this);
	TT->Insert_Collider();

	m_UList.push_back(TT);

	return TT;
}

KPtr<Force_Unit> SC2_Force::Find_Unit(const wchar_t* _Name)
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		if (true == PathManager::Is_StrVSStr((*m_SUI)->name(), _Name))
		{
			return (*m_SUI);
		}
	}
	return nullptr;
}

void SC2_Force::Clear_Unit()
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		(*m_SUI)->one()->Set_Death();
	}

	m_UList.clear();
}

void SC2_Force::Delete_Unit(KPtr<Force_Unit> _Unit)
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		if ((*m_SUI) == _Unit)
		{
			(*m_SUI)->one()->Set_Death();
			m_UList.erase(m_SUI);
			return;
		}
	}
}


bool SC2_Force::Init()
{
	return true;
}


void SC2_Force::Update()
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI;)
	{
		if (false == (*m_SUI)->Is_Death())
		{
			(*m_SUI)->Update();
			++m_SUI;
		}
		else
		{
			m_SUI = m_UList.erase(m_SUI);
		}
	}
}



// 해당 세력의 유닛 배치
void SC2_Force::Save()
{
	std::wstring Tmp = ws_name();
	Tmp += L".force";

	Tmp = PathManager::Find_ForderPath(L"FORCE") + Tmp;


	// 개수
	// 유닛 이름 - 위치
	WriteStream WS = WriteStream(Tmp.c_str());


	int Size = (int)m_UList.size();
	WS.Write(Size);


	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		KVector TT = (*m_SUI)->one()->Trans()->pos_world();
		WS.Write((*m_SUI)->name(), sizeof(wchar_t) * NAMENUM);
		WS.Write(&TT, sizeof(KVector));
	}
}


void SC2_Force::Load(const wchar_t* _Name, KPtr<Renderer_Terrain> _Ter)
{
	Clear_Unit();


	name(_Name);
	std::wstring Tmp = _Name;
	Tmp += L".force";
	Tmp = PathManager::Find_ForderPath(L"FORCE") + Tmp;

	ReadStream RS = ReadStream(Tmp.c_str());


	// 개수
	// 유닛 이름 - 위치

	int Size = 0;
	RS.Read(Size);


	for (int i = 0; i < Size; i++)
	{
		wchar_t Tmp[NAMENUM];
		KVector TT;
		RS.Read(Tmp, sizeof(wchar_t) * NAMENUM);
		RS.Read(&TT, sizeof(KVector));

		KPtr<Force_Unit> TU = Create_Unit(Tmp, _Ter);
		TU->one()->Trans()->pos_local(TT);
	}
}


void SC2_Force::playable_type(const PLAYABLE_TYPE& _Value)
{
	m_SUI = m_UList.begin();
	m_EUI = m_UList.end();

	for (; m_SUI != m_EUI; ++m_SUI)
	{
		(*m_SUI)->playable_type(_Value);

		switch (_Value)
		{
		case PBT_NONE:
		{	
			(*m_SUI)->Delete_Component<Controll_AI>();
			(*m_SUI)->Delete_Component<Controll_User>();
			break;
		}
		case PBT_ENEMY:
		{
			(*m_SUI)->Add_Component<Controll_AI>((*m_SUI));
			break;
		}
		case PBT_USER:
			break;
		default:
			break;
		}
	}
}
PLAYABLE_TYPE& SC2_Force::playable_type()
{
	return (*m_UList.begin())->playable_type();
}