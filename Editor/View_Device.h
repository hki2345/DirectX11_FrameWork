#pragma once


// View_Device ���Դϴ�.

class View_Device : public CView
{
	DECLARE_DYNCREATE(View_Device)

protected:
	View_Device();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~View_Device();

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


