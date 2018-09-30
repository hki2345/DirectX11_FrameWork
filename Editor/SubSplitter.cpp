#include "stdafx.h"
#include "SubSplitter.h"


SubSplitter::SubSplitter() : m_bLock(false)
{
}


SubSplitter::~SubSplitter()
{
}

// 해당 클래스에 이 클래스 안 이런 것들을 쓸 것이다.
BEGIN_MESSAGE_MAP(SubSplitter, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()

	// 마지막에 이거 넣어주어 마무리 } 중괄호를 닫거나 함수를 마무리 지어줌
END_MESSAGE_MAP()


// 오버라이드 하여 원하는 함수를 커스터마이징 하는 식이다.
void SubSplitter::OnLButtonDown(UINT nFlages, CPoint point)
{
	if (true == m_bLock)
	{
		return;
	}

	CSplitterWnd::OnLButtonDown(nFlages, point);
}

void SubSplitter::OnMouseMove(UINT nFlages, CPoint point)
{
	if (true == m_bLock)
	{
		return;
	}

	CSplitterWnd::OnMouseMove(nFlages, point);
}

BOOL SubSplitter::OnSetCursor(CWnd* _pWnd, UINT _nHitTest, UINT _Msg)
{
	if (true == m_bLock)
	{
		return TRUE;
	}

	return CSplitterWnd::OnSetCursor(_pWnd, _nHitTest, _Msg);
}

BOOL SubSplitter::PreCreateWindow(CREATESTRUCT& cs)
{
	// 부모의 값 -> 덮어씌우는 형식 오버라이드 해서
	// 창을 바뀨거나 수정하는 식이다.
	m_cxSplitter = 1;
	m_cySplitter = 1;

	//m_cxBorder = 5;
	//m_cyBorder = 5;

	// 이 수는 0 이하가 되면 에러 -> 등분 하는 선이 0 이하라는 소리
	m_cxSplitterGap = 1;
	m_cySplitterGap = 1;

	return CSplitterWnd::PreCreateWindow(cs);
}