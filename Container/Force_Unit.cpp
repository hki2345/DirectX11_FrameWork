#include "Force_Unit.h"
#include "SC2_Force.h"


#include <Core_Class.h>

#include <ReadStream.h>
#include <WriteStream.h>

#include <ResourceManager.h>
#include <Renderer_BonAni.h>
#include <KSphere_Col.h>


Force_Unit::Force_Unit()
{
	m_Info.WTpye = WEAPON_TYPE::NONE;
	m_Info.LSpeed = .0f;
	m_Info.RSpeed = .0f;
	m_Info.UScale = KVector::Zero;
}


Force_Unit::~Force_Unit()
{
}

bool Force_Unit::Init(const wchar_t* _Name)
{
	Load(_Name);

	return true;
}

void Force_Unit::Update()
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		if (nullptr != m_Force)
		{
			(*m_SRI)->force_color(m_Force->force_color());
		}
	}
}


// 리소스 메니저에서 이미 All_LOad를 하였다 가정한다.
bool Force_Unit::Load(const wchar_t* _Name)
{
	Reset_Renderer();
	name(_Name);
	std::wstring Tmp = _Name;
	Tmp += L".KUD";

	Tmp = PathManager::Find_ForderPath(L"KUD") + Tmp;

	ReadStream RS = ReadStream(Tmp.c_str());
	

	RS.Read(m_Info.WTpye);
	RS.Read(m_Info.LSpeed);
	RS.Read(m_Info.RSpeed);
	RS.Read(m_Info.UScale);

	int Cnt = 0;
	RS.Read(Cnt);
	for (size_t i = 0; i < Cnt; i++)
	{
		wchar_t Tmp[NAMENUM];
		RS.Read(Tmp, sizeof(wchar_t) * NAMENUM);
		m_StrList.push_back(Tmp);
	}



	m_SCI = m_StrList.begin();
	m_ECI = m_StrList.end();

	for (; m_SCI != m_ECI; ++m_SCI)
	{
		Tmp.clear();
		Tmp += (*m_SCI) + L".KM3";

		if (nullptr == one())
		{
			KPtr<TheOne> TOne = Core_Class::MainScene()->Create_One(L"TKM3");
			TOne->Set_Component(this);
		}

		
		KPtr<Renderer_BonAni> TRen = one()->Add_Component<Renderer_BonAni>();
		TRen->Set_Fbx(Tmp.c_str());
		TRen->Create_Animation();
		m_RList.push_back(TRen);
	}


	return true;
}

bool Force_Unit::Save()
{
	std::wstring Tmp = ws_name();
	Tmp += L".KUD";

	Tmp = PathManager::Find_ForderPath(L"KUD") + Tmp;
	WriteStream WS = WriteStream(Tmp.c_str());

	WS.Write(m_Info.WTpye);
	WS.Write(m_Info.LSpeed);
	WS.Write(m_Info.RSpeed);
	WS.Write(m_Info.UScale);

	int Cnt = (int)m_StrList.size();
	WS.Write(Cnt);



	m_SCI = m_StrList.begin();
	m_ECI = m_StrList.end();

	for (; m_SCI != m_ECI; ++m_SCI)
	{
		WS.Write((*m_SCI).c_str(), sizeof(wchar_t) * NAMENUM);
	}

	return true;
}

void Force_Unit::Reset_Renderer()
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		(*m_SRI)->Set_Death();
	}

	m_RList.clear();
	m_StrList.clear();
	Update_StrList();
}


void Force_Unit::Delete_Renderer(KPtr<Renderer_BonAni> _Other)
{
	if (nullptr == _Other)
	{
		BBY;
	}

	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();


	for (; m_SRI != m_ERI; ++m_SRI)
	{
		if ((*m_SRI) == _Other)
		{
			m_RList.erase(m_SRI);
			break;
		}
	}
	Update_StrList();
}

void Force_Unit::Insert_Renderer(KPtr<Renderer_BonAni> _Other)
{
	if (nullptr == _Other)
	{
		BBY;
	}

	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		if ((*m_SRI) == _Other)
		{
			return;
		}
	}
	
	m_RList.push_back(_Other);
	Update_StrList();
}

void Force_Unit::Update_StrList()
{
	m_StrList.clear();
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		m_StrList.push_back((*m_SRI)->mesh_container()->FileName());
	}
}