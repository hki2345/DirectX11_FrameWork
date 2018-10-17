// SpriteSelectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "SpriteSelectDlg.h"
#include "afxdialogex.h"
#include <HResMgr.h>
#include <HImage.h>


// SpriteSelectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(SpriteSelectDlg, CDialogEx)

SpriteSelectDlg::SpriteSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SPRITESELECT, pParent)
{

}

SpriteSelectDlg::~SpriteSelectDlg()
{
	for (size_t i = 0; i < m_ButtonVector.size(); i++)
	{
		delete m_ButtonVector[i];
	}
}

void SpriteSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SpriteSelectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_CONTROL_RANGE(BN_CLICKED, 0, 10000, &SpriteSelectDlg::ButtonClick)
END_MESSAGE_MAP()


// SpriteSelectDlg 메시지 처리기입니다.


BOOL SpriteSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_SampleSize = { 100, 100 };

	m_SlotSize = { 100, 120 };

	CountX = 5;


	// m_ImageList.inser

	RECT Rc = { 0, 0, m_SampleSize.cx * (LONG)CountX
		, m_SlotSize.cy * (LONG)((UINT)HResMgr<HImage>::AllResCount() / CountX + 1) };

	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(nullptr, 0, 0, Rc.right - Rc.left
		, Rc.bottom - Rc.top, SWP_NOMOVE | SWP_NOZORDER);

	int IdCout = 1;
	int Count = 0;

	POINT StartPoint = { 0, 100 };
	SIZE BtnSize = { 100, 20 };
	RECT m_BtnRect = { 0, 0, 100, 100 };

	std::list<HPTR<HImage>> List = HResMgr<HImage>::GetAllSingleResList();
	std::list<HPTR<HImage>>::iterator ImageStartIter;
	std::list<HPTR<HImage>>::iterator ImageEndIter;

	ImageStartIter = List.begin();
	ImageEndIter = List.end();

	m_ButtonVector.reserve(List.size());

	for (; ImageStartIter != ImageEndIter; ++ImageStartIter)
	{
		ImageSelectButton* p = new ImageSelectButton();

		p->m_ImageKey = (*ImageStartIter)->FullFileName();
		CRect BtnRc = { StartPoint.x, StartPoint.y, StartPoint.x + BtnSize.cx, StartPoint.y + BtnSize.cy };
		m_ButtonVector.push_back(p);
		p->Create(L"선택", BS_DEFPUSHBUTTON, BtnRc, this, IdCout++);
		p->SetWindowTextW(L"선택");
		p->ShowWindow(SW_SHOW);

		++Count;
		StartPoint.x += m_SampleSize.cx;

		if (Count >= (int)CountX)
		{
			Count = 0;
			StartPoint.x = 0;
			StartPoint.y += m_SlotSize.cy;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void SpriteSelectDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	std::list<HPTR<HImage>> List = HResMgr<HImage>::GetAllSingleResList();

	std::list<HPTR<HImage>>::iterator ImageStartIter;
	std::list<HPTR<HImage>>::iterator ImageEndIter;

	ImageStartIter = List.begin();
	ImageEndIter = List.end();

	POINT StartPoint = {0, 0};
	RECT m_SrcRect = { 0, 0, 100, 100 };

	int Count = 0;

	for (; ImageStartIter != ImageEndIter; ++ImageStartIter)
	{
		CImage Image;
		RECT m_DestRect = { 0, 0, (int)(*ImageStartIter)->GetTex()->Width(), (int)(*ImageStartIter)->GetTex()->Height() };
		CString Path = (*ImageStartIter)->Path();
		Image.Load(Path);
		m_SrcRect = { StartPoint .x, StartPoint .y, StartPoint.x + m_SampleSize.cx, StartPoint.y + m_SampleSize.cy };
		Image.AlphaBlend(dc.m_hDC, m_SrcRect, m_DestRect);

		++Count;
		StartPoint.x += m_SampleSize.cx;

		if (Count >= (int)CountX)
		{
			Count = 0;
			StartPoint.x = 0;
			StartPoint.y += m_SlotSize.cy;
		}

	}
}


void SpriteSelectDlg::ButtonClick(UINT _Id) 
{
	ImageName = m_ButtonVector[_Id - 1]->m_ImageKey;

	OnCancel();
}