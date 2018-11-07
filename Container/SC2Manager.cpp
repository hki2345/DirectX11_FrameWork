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
			// force ��ü�� ���� �� �ִ�. 
			// ���������� ������ ������ �ִϸ��̼� �� ����� ��� ��
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

