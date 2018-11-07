#include "SC2Manager.h"
#include "SC2_Force.h"
#include <KMacro.h>



SC2Manager::SC2Manager() : m_bGR(GRT_PLAY)
{
}


SC2Manager::~SC2Manager()
{
}


bool SC2Manager::Init()
{
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

