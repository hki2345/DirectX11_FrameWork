#include "stdafx.h"
#include "AddCom.h"
#include <HSpRenderer.h>
#include <HCol2D.h>


void AddCom::CreateCom(CString ComName) 
{
	if (ComName == L"HSpRenderer")
	{
		m_pActor->AddCom<HSpRenderer>();
	}
	else if (ComName == L"HCol2D")
	{
		m_pActor->AddCom<HCol2D>();
	}
	else {
		AfxMessageBox(L"�߰��� �� ���� ������Ʈ �Դϴ�");
	}

}