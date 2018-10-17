// SpriteSelectDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "SpriteSelectDlg.h"
#include "afxdialogex.h"
#include <HResMgr.h>
#include <HImage.h>


// SpriteSelectDlg ��ȭ �����Դϴ�.

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


// SpriteSelectDlg �޽��� ó�����Դϴ�.


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
		p->Create(L"����", BS_DEFPUSHBUTTON, BtnRc, this, IdCout++);
		p->SetWindowTextW(L"����");
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void SpriteSelectDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
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