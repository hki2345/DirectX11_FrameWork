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
		AfxMessageBox(L"추가할 수 없는 컴포넌트 입니다");
	}

}