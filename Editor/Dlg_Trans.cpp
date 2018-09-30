// Dlg_Trans.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Editor.h"
#include "Dlg_Trans.h"
#include "afxdialogex.h"
#include "Edit_Class.h"

// Dlg_Trans 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_Trans, Dlg_Component)

Dlg_Trans::Dlg_Trans(CWnd* pParent /*=NULL*/)
	: Dlg_Component(IDD_TRANS, pParent), m_Trans(nullptr)
{
	Y_SIZE = 180;
	Edit_Class::TransTab = this;
}

Dlg_Trans::~Dlg_Trans()
{
	Edit_Class::TransTab = nullptr;
}

void Dlg_Trans::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	int StartId = IDC_XPOS;

	for (size_t Y = 0; Y < 3; Y++)
	{
		for (size_t X = 0; X < 3; X++)
		{
			DDX_Control(pDX, StartId, TransMatValue[Y][X]);
			DDX_Text(pDX, StartId, TransMat.m[Y][X]);
			++StartId;
		}
	}


	StartId = IDC_XPOS2;

	for (size_t Y = 0; Y < 3; Y++)
	{
		for (size_t X = 0; X < 3; X++)
		{
			TransDroper[Y][X].m_Value = &TransMat.m[Y][X];
			TransDroper[Y][X].m_Parent = this;
			TransDroper[Y][X].Drop_ValueChager(this, &Dlg_Trans::Update_Trans);
			DDX_Control(pDX, StartId, TransDroper[Y][X]);

			switch (X)
			{
			case 0:
				TransDroper[Y][X].SetWindowTextW(L"X");
				break;
			case 1:
				TransDroper[Y][X].SetWindowTextW(L"Y");
				break;
			case 2:
				TransDroper[Y][X].SetWindowTextW(L"Z");
				break;
			default:
				break;
			}

			++StartId;
		}
	}
}


BEGIN_MESSAGE_MAP(Dlg_Trans, CDialogEx)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_XPOS, IDC_ZSCALE, &Dlg_Trans::OnEnFocusTransValue)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_XPOS, IDC_ZSCALE, &Dlg_Trans::OnEnChangeTransValue)
END_MESSAGE_MAP()


// Dlg_Trans 메시지 처리기입니다.


void Dlg_Trans::OnEnFocusTransValue(UINT _Id)
{
}

void Dlg_Trans::OnEnChangeTransValue(UINT _Id)
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// ComDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	int ind = _Id - IDC_XPOS;
	int indX = ind % 3;
	int indY = ind / 3;

	bool check = false;

	// 짱나게 0 일때 데이터 최신화를 하면 커서가 앞으로 이동함 ㅋㅋㅋㅋ
	if ( .0f == TransMat.m[indY][indX])
	{
		check = true;
	}

	UpdateData(TRUE);
	UpdateData(FALSE);

	if (true == check)
	{
		TransMatValue[indY][indX].SetSel(1, 1);
	}

	Update_Trans();
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void Dlg_Trans::set_component(Component* _Com)
{
	if (_Com->typeinfo() != &typeid(TransPosition))
	{
		return;
	}

	m_Trans = (TransPosition*)_Com;

	UpdateData(TRUE);

	KVector3 Temp;
	Temp = m_Trans->Local_Pos().m_Vector3;
	memcpy(&TransMat.vec1, &Temp, sizeof(KVector3));
	Temp = m_Trans->Local_Rotate().m_Vector3;
	memcpy(&TransMat.vec2, &Temp, sizeof(KVector3));
	Temp = m_Trans->Local_Scale().m_Vector3;
	memcpy(&TransMat.vec3, &Temp, sizeof(KVector3));
	
	UpdateData(FALSE);
}

void Dlg_Trans::Update_Trans()
{
	m_Trans->Local_Pos(TransMat.vec1);
	m_Trans->Local_Rotate(TransMat.vec2);
	m_Trans->Local_Scale(TransMat.vec3);
}

void Dlg_Trans::Update_EditValue()
{
	UpdateData(TRUE);

	KVector3 Temp;
	Temp = m_Trans->Local_Pos().m_Vector3;
	memcpy(&TransMat.vec1, &Temp, sizeof(KVector3));
	Temp = m_Trans->Local_Rotate_Degree().m_Vector3;
	memcpy(&TransMat.vec2, &Temp, sizeof(KVector3));
	Temp = m_Trans->Local_Scale().m_Vector3;
	memcpy(&TransMat.vec3, &Temp, sizeof(KVector3));

	UpdateData(FALSE);
}
