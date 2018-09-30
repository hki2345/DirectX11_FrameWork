#pragma once
#include <TheOne.h>
#include <list>
#include "Dlg_Component.h"

// View_Component 뷰입니다.

class View_Component : public CScrollView
{
	DECLARE_DYNCREATE(View_Component)

protected:
	View_Component();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~View_Component();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

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


