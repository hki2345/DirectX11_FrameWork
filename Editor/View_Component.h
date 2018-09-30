#pragma once
#include <TheOne.h>
#include <list>
#include "Dlg_Component.h"

// View_Component ���Դϴ�.

class View_Component : public CScrollView
{
	DECLARE_DYNCREATE(View_Component)

protected:
	View_Component();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~View_Component();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual void OnInitialUpdate();     // ������ �� ó���Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	TheOne* m_One;
	std::list<Dlg_Component*>::iterator S_DlgIter;
	std::list<Dlg_Component*>::iterator E_DlgIter;
	std::list<Dlg_Component*> m_DlgCom_list;

public:
	void set_one(TheOne* _Value);
	void Dlg_Release();
	void reset();

	Dlg_Component* create_comDlg(const type_info* _type);
};


