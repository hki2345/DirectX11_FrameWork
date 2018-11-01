#include "stdafx.h"
#include "AddCom.h"
#include <HSpRenderer.h>
#include <KCollider2D.h>


void AddCom::CreateCom(CString ComName) 
{
	if (ComName == L"HSpRenderer")
	{
		m_pActor->Add_Component<HSpRenderer>();
	}
	else if (ComName == L"KCollider2D")
	{
		m_pActor->Add_Component<KCollider2D>();
	}
	else {
		AfxMessageBox(L"�߰��� �� ���� ������Ʈ �Դϴ�");
	}

}