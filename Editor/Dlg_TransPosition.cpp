// Dlg_TransPosition.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEditor.h"
#include "Dlg_TransPosition.h"
#include "afxdialogex.h"


// Dlg_TransPosition 대화 상자입니다.

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


// Dlg_TransPosition 메시지 처리기입니다.


void Dlg_TransPosition::OnEnFocusTransValue(UINT _Id) {
	int a = 0;
}

void Dlg_TransPosition::OnEnChangeTransValue(UINT _Id)
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// ComDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	int Index = _Id - IDC_POSX;
	int IndexX = Index % 3;
	int IndexY = Index / 3;

	bool Check = false;

	// 짱나게 0 일때 데이터 최신화를 하면 커서가 앞으로 이동함 ㅋㅋㅋㅋ
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
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
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




//-CFileDialog 정의
//CFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, 
//	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWITEPROMPT, 
//	OPCTSTR lpszFilter = NULL, CWnd *pParentWnd = NULL)

//-생성자 파라미터
//bOpenFileDialog : TRUE:열기, FALSE : 다른 이름으로저장
//	lpszDefExt : 기본 확장자. (ex. "*.jpg")
//	lpszFileName : 처음 대화상자가 출력 시 '파일 이름(N)'에 지정될 파일명
//	dwFlags :
//
//OFN_EXPLOPER: 윈도우 탐색기 스타일로 출력
//	OFN_ALLOWMULTISELECT : 다중 선택
//	OFN_CREATEPROMPT : 존재하지 않는 파일명 입력 시, 새로운 파일을 생성할 것인지 물음
//	OFN_FILEMUSTEXIST : 존재하지 않는 파일명 입력 불가
//	OFN_HIDEREADONLY : 읽기전용 파일은 보이지 않음
//	OFN_LONGNAMES : 긴 파일 이름 포맷 지원
//	OFN_OVERWRITEPROMPT : 이미 존재하는 파일 명일 경우 덮어쓸 것인가를 물음
//	OFN_PATHMUSTEXIST : 이미 존재하는 디렉터리명만을 입력
//	lpszFilter : 보여질 파일들에 대한 필터
//	pParentWnd : 대화상자의 부모 윈도우 지정
//
//
//	-멤버 함수
//	CString GetPathName() : 선택된 파일의 절대경로
//	CString GetFileName() : 선택된 파일의 이름과 확장자
//	CString GetFileExt() : 선택된 파일의 확장자
//	String GetFileTitle() : 선택된 파일의 파일명
//	BOOL GetReadOnlyPref() : 읽기전용 여부
//	POSITION GetStartPosition() : 다중 선택시 첫번째 파일의 위치
//	CString GetNextPathName() : 다중 선택시 다음 파일의 절대경로
//  다중 사용 문제점 
// https://m.blog.naver.com/PostView.nhn?blogId=jalaint&logNo=150073298476&proxyReferer=http%3A%2F%2Fwww.google.com%2Furl%3Fsa%3Dt%26rct%3Dj%26q%3D%26esrc%3Ds%26source%3Dweb%26cd%3D4%26ved%3D2ahUKEwij-v6otO_eAhVTQLwKHRBODwkQFjADegQIBhAB%26url%3Dhttp%253A%252F%252Fm.blog.naver.com%252Fjalaint%252F150073298476%26usg%3DAOvVaw2S-tLlWcpNhuAf2fJeXYXj

void Dlg_TransPosition::OnBnClickedButton1()
{
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");

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

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int a = 9;
}