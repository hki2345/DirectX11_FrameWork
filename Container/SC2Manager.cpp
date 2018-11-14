#include "SC2Manager.h"
#include "SC2_Force.h"
#include <KMacro.h>



SC2Manager::SC2Manager() : m_bGR(GRT_PLAY)
{
}


SC2Manager::~SC2Manager()
{
}


KPtr<SC2_Force> SC2Manager::Create_Force(const wchar_t* _Name, const KColor& _Color)
{
	m_SFI = m_FMap.find(_Name);

	if (m_FMap.end() != m_SFI)
	{
		return m_SFI->second;
	}

	KPtr<SC2_Force> NewForce = new SC2_Force(_Name, _Color);

	if (nullptr != NewForce)
	{
		m_FMap.insert(std::map<std::wstring, KPtr<SC2_Force>>::value_type(_Name, NewForce));
	}


	return NewForce;
}




KPtr<SC2_Force> SC2Manager::Find_Force(const wchar_t* _Name)
{
	m_SFI = m_FMap.find(_Name);
	
	if (m_FMap.end() == m_SFI)
	{
		return nullptr;
	}

	return m_SFI->second;
}

KPtr<SC2_Force> SC2Manager::Find_Force(const KColor& _Color)
{
	m_SFI = m_FMap.begin();
	m_EFI = m_FMap.begin();

	for (; m_SFI != m_EFI; ++m_SFI)
	{
		if (_Color == m_SFI->second->m_Color)
		{
			return m_SFI->second;
		}
	}

	return nullptr;
}


bool SC2Manager::Init()
{
	Create_Force(L"RED", KColor::Red);
	Create_Force(L"BLUE", KColor::Blue);
	Create_Force(L"GREEN", KColor::Green);
	Create_Force(L"YELLOW", KColor::Yellow);
	Create_Force(L"WHITE", KColor::White);
	Create_Force(L"CYAN", KColor::Cyan);
	Create_Force(L"BROWN", KColor::Red);
	Create_Force(L"ORANGE", KColor::Red);


	return true;
}




void SC2Manager::Update()
{
	switch (m_bGR)
	{
	case SC2Manager::GRT_PAUSE:
		break;
	case SC2Manager::GRT_PLAY:
	{
		m_SFI = m_FMap.begin();
		m_EFI = m_FMap.begin();

		for (; m_SFI !=m_EFI; ++m_SFI)
		{
			// force 전체가 멈출 수 있다. 
			// 게임적으로 멈추지 랜더나 애니메이션 및 사운드는 계속 됌
			if (true == m_SFI->second->Is_Active())
			{
				m_SFI->second->Update();
			}
		}

		break;
	}

	case SC2Manager::GRT_ERROR:
		KASSERT(true);
		break;
	default:
		break;
	}
}

