#pragma once


// View_Main_Frame ���Դϴ�.

class View_Main_Frame : public CView
{
	DECLARE_DYNCREATE(View_Main_Frame)

protected:
	View_Main_Frame();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~View_Main_Frame();

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
};


