#include "stdafx.h"
#include "SubSplitter.h"


SubSplitter::SubSplitter() : m_bLock(false)
{
}


SubSplitter::~SubSplitter()
{
}

// �ش� Ŭ������ �� Ŭ���� �� �̷� �͵��� �� ���̴�.
BEGIN_MESSAGE_MAP(SubSplitter, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()

	// �������� �̰� �־��־� ������ } �߰�ȣ�� �ݰų� �Լ��� ������ ������
END_MESSAGE_MAP()


// �������̵� �Ͽ� ���ϴ� �Լ��� Ŀ���͸���¡ �ϴ� ���̴�.
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
	// �θ��� �� -> ������ ���� �������̵� �ؼ�
	// â�� �ٲ�ų� �����ϴ� ���̴�.
	m_cxSplitter = 1;
	m_cySplitter = 1;

	//m_cxBorder = 5;
	//m_cyBorder = 5;

	// �� ���� 0 ���ϰ� �Ǹ� ���� -> ��� �ϴ� ���� 0 ���϶�� �Ҹ�
	m_cxSplitterGap = 1;
	m_cySplitterGap = 1;

	return CSplitterWnd::PreCreateWindow(cs);
}