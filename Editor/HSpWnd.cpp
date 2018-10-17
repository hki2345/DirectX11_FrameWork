#include "stdafx.h"
#include "HSpWnd.h"


HSpWnd::HSpWnd() : m_bLock(false)
{
}


HSpWnd::~HSpWnd()
{
}

BEGIN_MESSAGE_MAP(HSpWnd, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void HSpWnd::OnLButtonDown(UINT nFlages, CPoint point) 
{
	if (true == m_bLock)
	{
		return;
	}

	CSplitterWnd::OnLButtonDown(nFlages, point);
}

void HSpWnd::OnMouseMove(UINT nFlages, CPoint point) 
{
	if (true == m_bLock)
	{
		return;
	}

	CSplitterWnd::OnMouseMove(nFlages, point);
}

BOOL HSpWnd::OnSetCursor(CWnd* _pWnd, UINT _nHitTest, UINT _Msg) 
{
	if (true == m_bLock)
	{
		return 0;
	}

	return CSplitterWnd::OnSetCursor(_pWnd, _nHitTest, _Msg);
}

BOOL HSpWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	m_cxSplitter = 1;
	m_cySplitter = 1;

	//m_cxBorder = 1;
	//m_cyBorder = 1;

	//// 외곽 
	//m_cxBorder = 5;
	//m_cyBorder = 5;

	// 이녀석들은 1 이상이어야 한다.
	m_cxSplitterGap = 1;
	m_cySplitterGap = 1;

	return CSplitterWnd::PreCreateWindow(cs);
}