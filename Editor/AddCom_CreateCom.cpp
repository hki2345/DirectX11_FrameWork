#include "stdafx.h"
#include "AddCom.h"
#include <HSpRenderer.h>
#include <HCol2D.h>


void AddCom::CreateCom(CString ComName) 
{
	if (ComName == L"HSpRenderer")
	{
		m_pActor->Add_Component<HSpRenderer>();
	}
	else if (ComName == L"HCol2D")
	{
		m_pActor->Add_Component<HCol2D>();
	}
	else {
		AfxMessageBox(L"�߰��� �� ���� ������Ʈ �Դϴ�");
	}

}