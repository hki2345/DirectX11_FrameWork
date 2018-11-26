// Dlg_TransPosition.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEditor.h"
#include "Dlg_TransPosition.h"
#include "afxdialogex.h"


// Dlg_TransPosition ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Dlg_TransPosition, ComDlg)

Dlg_TransPosition::Dlg_TransPosition(CWnd* pParent /*=NULL*/)
	: ComDlg(IDD_TRANSCOM, pParent), m_pTrans(nullptr)
{
	SIZEY = 180;
}

Dlg_TransPosition::~Dlg_TransPosition()
{
}

void Dlg_TransPosition::DoDataExchange(CDataExchange* pDX)
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
			NameXYZ[Y][X].ValueChangeFunc(this, &Dlg_TransPosition::Update_Trans);
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


BEGIN_MESSAGE_MAP(Dlg_TransPosition, CDialogEx)
	ON_BN_CLICKED(IDC_TESTBTN, &Dlg_TransPosition::OnBnClickedButton1)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_POSX, IDC_SCALZ, &Dlg_TransPosition::OnEnFocusTransValue)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_POSX, IDC_SCALZ, &Dlg_TransPosition::OnEnChangeTransValue)
END_MESSAGE_MAP()


// Dlg_TransPosition �޽��� ó�����Դϴ�.


void Dlg_TransPosition::OnEnFocusTransValue(UINT _Id) {
	int a = 0;
}

void Dlg_TransPosition::OnEnChangeTransValue(UINT _Id)
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// ComDlg::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	int Index = _Id - IDC_POSX;
	int IndexX = Index % 3;
	int IndexY = Index / 3;

	bool Check = false;

	// ¯���� 0 �϶� ������ �ֽ�ȭ�� �ϸ� Ŀ���� ������ �̵��� ��������
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
	Update_Trans();

	// 
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void Dlg_TransPosition::SettingComponent(Component* _pCom) 
{
	if (_pCom->TypeInfo() != &typeid(TransPosition))
	{
		return;
	}

	m_pTrans = (TransPosition*)_pCom;

	UpdateData(TRUE);


	KVector3 Temp;
	Temp = m_pTrans->pos_local().m_Vec3;
	memcpy(&TransValue.v1, &Temp, sizeof(KVector3));
	Temp = m_pTrans->rotate_local().m_Vec3;
	memcpy(&TransValue.v2, &Temp, sizeof(KVector3));
	Temp = m_pTrans->scale_local().m_Vec3;
	memcpy(&TransValue.v3, &Temp, sizeof(KVector3));

	UpdateData(FALSE);
}

void Dlg_TransPosition::Update_Trans() 
{

	m_pTrans->pos_local(TransValue.v1);
	m_pTrans->rotate_local(TransValue.v2);
	m_pTrans->scale_local(TransValue.v3);
}




//-CFileDialog ����
//CFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, 
//	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWITEPROMPT, 
//	OPCTSTR lpszFilter = NULL, CWnd *pParentWnd = NULL)

//-������ �Ķ����
//bOpenFileDialog : TRUE:����, FALSE : �ٸ� �̸���������
//	lpszDefExt : �⺻ Ȯ����. (ex. "*.jpg")
//	lpszFileName : ó�� ��ȭ���ڰ� ��� �� '���� �̸�(N)'�� ������ ���ϸ�
//	dwFlags :
//
//OFN_EXPLOPER: ������ Ž���� ��Ÿ�Ϸ� ���
//	OFN_ALLOWMULTISELECT : ���� ����
//	OFN_CREATEPROMPT : �������� �ʴ� ���ϸ� �Է� ��, ���ο� ������ ������ ������ ����
//	OFN_FILEMUSTEXIST : �������� �ʴ� ���ϸ� �Է� �Ұ�
//	OFN_HIDEREADONLY : �б����� ������ ������ ����
//	OFN_LONGNAMES : �� ���� �̸� ���� ����
//	OFN_OVERWRITEPROMPT : �̹� �����ϴ� ���� ���� ��� ��� ���ΰ��� ����
//	OFN_PATHMUSTEXIST : �̹� �����ϴ� ���͸����� �Է�
//	lpszFilter : ������ ���ϵ鿡 ���� ����
//	pParentWnd : ��ȭ������ �θ� ������ ����
//
//
//	-��� �Լ�
//	CString GetPathName() : ���õ� ������ ������
//	CString GetFileName() : ���õ� ������ �̸��� Ȯ����
//	CString GetFileExt() : ���õ� ������ Ȯ����
//	String GetFileTitle() : ���õ� ������ ���ϸ�
//	BOOL GetReadOnlyPref() : �б����� ����
//	POSITION GetStartPosition() : ���� ���ý� ù��° ������ ��ġ
//	CString GetNextPathName() : ���� ���ý� ���� ������ ������
//  ���� ��� ������ 
// https://m.blog.naver.com/PostView.nhn?blogId=jalaint&logNo=150073298476&proxyReferer=http%3A%2F%2Fwww.google.com%2Furl%3Fsa%3Dt%26rct%3Dj%26q%3D%26esrc%3Ds%26source%3Dweb%26cd%3D4%26ved%3D2ahUKEwij-v6otO_eAhVTQLwKHRBODwkQFjADegQIBhAB%26url%3Dhttp%253A%252F%252Fm.blog.naver.com%252Fjalaint%252F150073298476%26usg%3DAOvVaw2S-tLlWcpNhuAf2fJeXYXj

void Dlg_TransPosition::OnBnClickedButton1()
{
	static TCHAR BASED_CODE szFilter[] = _T("�̹��� ����(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |�������(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, szFilter, this);

	if (IDOK == dlg.DoModal())
	{
		POSITION pos = dlg.GetStartPosition();
		CString m_FileList = L"";

		while (pos)
		{
			CString pathName = dlg.GetNextPathName(pos);

			pathName.Delete(0, pathName.ReverseFind('\\' + 1));
			m_FileList += (pathName + _T("\r\n"));
			MessageBox(pathName);
		}
	}

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int a = 9;
}