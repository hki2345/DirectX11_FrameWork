#pragma once


// LeftView ���Դϴ�.

class LeftView : public CView
{
	DECLARE_DYNCREATE(LeftView)

protected:
	LeftView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~LeftView();

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

