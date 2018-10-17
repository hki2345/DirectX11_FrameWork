#pragma once
#include "MenuDlg.h"

// RightView ���Դϴ�.

class RightView : public CView
{
	DECLARE_DYNCREATE(RightView)

protected:
	RightView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~RightView();

private:
	MenuDlg MDlg;

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};


