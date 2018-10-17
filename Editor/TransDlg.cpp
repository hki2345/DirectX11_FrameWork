// TransDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "TransDlg.h"
#include "afxdialogex.h"


// TransDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(TransDlg, ComDlg)

TransDlg::TransDlg(CWnd* pParent /*=NULL*/)
	: ComDlg(IDD_TRANSCOM, pParent), m_pTrans(nullptr)
{
	SIZEY = 180;
}

TransDlg::~TransDlg()
{
}

void TransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	int StartId = IDC_POSX;

	for (size_t Y = 0; Y < 3; Y++)
	{
		for (size_t X = 0; X < 3; X++)
		{
			DDX_Control(pDX, StartId, ArrMatValue[Y][X]);
			DDX_Text(pDX, StartId, TransValue.m[Y][X]);
			++StartId;
		}
	}


	StartId = IDC_POSX2;

	// NameXYZ[0][0].ONSET

	for (size_t Y = 0; Y < 3; Y++)
	{
		for (size_t X = 0; X < 3; X++)
		{
			NameXYZ[Y][X].pValue = &TransValue.m[Y][X];
			NameXYZ[Y][X].Parent = this;
			NameXYZ[Y][X].ValueChangeFunc(this, &TransDlg::TransUpdate);
			DDX_Control(pDX, StartId, NameXYZ[Y][X]);

			switch (X)
			{
			case 0:
				NameXYZ[Y][X].SetWindowTextW(L"X");
				break;
			case 1:
				NameXYZ[Y][X].SetWindowTextW(L"Y");
				break;
			case 2:
				NameXYZ[Y][X].SetWindowTextW(L"Z");
				break;
			default:
				break;
			}

			++StartId;
		}
	}
}


BEGIN_MESSAGE_MAP(TransDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &TransDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &TransDlg::OnBnClickedCancel)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_POSX, IDC_SCALZ, &TransDlg::OnEnFocusTransValue)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_POSX, IDC_SCALZ, &TransDlg::OnEnChangeTransValue)
END_MESSAGE_MAP()


// TransDlg �޽��� ó�����Դϴ�.


void TransDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void TransDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void TransDlg::OnEnFocusTransValue(UINT _Id) {
	int a = 0;
}

void TransDlg::OnEnChangeTransValue(UINT _Id)
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// ComDlg::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	int Index = _Id - IDC_POSX;
	int IndexX = Index % 3;
	int IndexY = Index / 3;

	bool Check = false;

	if (0.0f == TransValue.m[IndexY][IndexX])
	{
		Check = true;
		// ArrMatValue[IndexY][IndexX].SetSel();
	}


	UpdateData(TRUE);
	UpdateData(FALSE);

	if (true == Check)
	{
		ArrMatValue[IndexY][IndexX].SetSel(1, 1);
	}
	TransUpdate();

	// 
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void TransDlg::SettingComponent(HComponent* _pCom) 
{
	if (_pCom->TypeInfo() != &typeid(HTrans))
	{
		return;
	}

	m_pTrans = (HTrans*)_pCom;

	UpdateData(TRUE);


	HVEC3 Temp;
	Temp = m_pTrans->LPos().m_Vec3;
	memcpy(&TransValue.v1, &Temp, sizeof(HVEC3));
	Temp = m_pTrans->LRot().m_Vec3;
	memcpy(&TransValue.v2, &Temp, sizeof(HVEC3));
	Temp = m_pTrans->LScale().m_Vec3;
	memcpy(&TransValue.v3, &Temp, sizeof(HVEC3));

	UpdateData(FALSE);
}

void TransDlg::TransUpdate() 
{

	m_pTrans->LPos(TransValue.v1);
	m_pTrans->LRot(TransValue.v2);
	m_pTrans->LScale(TransValue.v3);
}