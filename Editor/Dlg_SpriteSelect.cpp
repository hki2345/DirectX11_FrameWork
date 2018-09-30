// Dlg_SpriteSelect.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Editor.h"
#include "Dlg_SpriteSelect.h"
#include "afxdialogex.h"
#include <ResourceManager.h>
#include <KImage.h>


// Dlg_SpriteSelect ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Dlg_SpriteSelect, CDialogEx)

Dlg_SpriteSelect::Dlg_SpriteSelect(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SPRITESELECT, pParent)
{

}

Dlg_SpriteSelect::~Dlg_SpriteSelect()
{
	for (size_t i = 0; i < m_ImageBtn_Vec.size(); ++i)
	{
		delete m_ImageBtn_Vec[i];
	}
}

void Dlg_SpriteSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg_SpriteSelect, CDialogEx)
	ON_WM_PAINT()
	ON_CONTROL_RANGE(BN_CLICKED, 0, 10000, &Dlg_SpriteSelect::ButtonClick)
END_MESSAGE_MAP()


// Dlg_SpriteSelect �޽��� ó�����Դϴ�.



BOOL Dlg_SpriteSelect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_SampleSize = { 100, 100 };

	m_SlotSize = { 100, 120 };

	CountX = 5;


	// m_ImageList.inser

	RECT Rc = { 0, 0, m_SampleSize.cx * (LONG)CountX
		, m_SlotSize.cy * (LONG)((UINT)ResourceManager<KImage>::All_Res_Count() / CountX + 1) };

	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(nullptr, 0, 0, Rc.right - Rc.left
		, Rc.bottom - Rc.top, SWP_NOMOVE | SWP_NOZORDER);

	int IdCout = 1;
	int Count = 0;

	POINT StartPoint = { 0, 100 };
	SIZE BtnSize = { 100, 20 };
	RECT m_BtnRect = { 0, 0, 100, 100 };



	std::list<KPtr<KImage>> List = ResourceManager<KImage>::All_SingleResList();
	std::list<KPtr<KImage>>::iterator ImageStartIter;
	std::list<KPtr<KImage>>::iterator ImageEndIter;

	ImageStartIter = List.begin();
	ImageEndIter = List.end();

	m_ImageBtn_Vec.reserve(List.size());

	for (; ImageStartIter != ImageEndIter; ++ImageStartIter)
	{
		Image_Selector* p = new Image_Selector();

		p->m_ImageKey = (*ImageStartIter)->file_name();
		CRect BtnRc = { StartPoint.x, StartPoint.y, StartPoint.x + BtnSize.cx, StartPoint.y + BtnSize.cy };
		m_ImageBtn_Vec.push_back(p);
		p->Create(L"Select", BS_DEFPUSHBUTTON, BtnRc, this, IdCout++);
		p->SetWindowTextW(L"Select");
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void Dlg_SpriteSelect::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	std::list<KPtr<KImage>> List = ResourceManager<KImage>::All_SingleResList();

	std::list<KPtr<KImage>>::iterator Img_StartIter;
	std::list<KPtr<KImage>>::iterator Img_EndIter;

	Img_StartIter = List.begin();
	Img_EndIter = List.end();

	POINT Start_Pnt = { 0, 0 };
	RECT m_SrcRect = { 0, 0, 100, 100 };

	int Count = 0;

	for (; Img_StartIter != Img_EndIter; ++Img_StartIter)
	{
		CImage NewImage;
		RECT m_DestRect = { 0, 0, (int)(*Img_StartIter)->texture()->width(), (int)(*Img_StartIter)->texture()->height() };
		CString Path = (*Img_StartIter)->path();
		NewImage.Load(Path);
		m_SrcRect = { Start_Pnt.x, Start_Pnt.y, Start_Pnt.x + m_SampleSize.cx, Start_Pnt.y + m_SampleSize.cy };
		NewImage.AlphaBlend(dc.m_hDC, m_SrcRect, m_DestRect);

		++Count;
		Start_Pnt.x += m_SampleSize.cx;

		if (Count >= (int)CountX)
		{
			Count = 0;
			Start_Pnt.x = 0;
			Start_Pnt.y += m_SlotSize.cy;
		}

	}
}


void Dlg_SpriteSelect::ButtonClick(UINT _Id) 
{
	Image_Name = m_ImageBtn_Vec[_Id - 1]->m_ImageKey;

	OnCancel();
}