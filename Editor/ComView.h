#pragma once
#include <TheOne.h>
#include <list>

#include "ComDlg.h"

// ComView 뷰입니다.

class ComView : public CScrollView
{
	DECLARE_DYNCREATE(ComView)

protected:
	ComView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ComView();

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
	TheOne* m_pActor;
	std::list<ComDlg*>::iterator StartDlgiter;
	std::list<ComDlg*>::iterator EndDlgiter;
	std::list<ComDlg*> m_ComDlgList;

public:
	ComDlg* CreateDlg(const type_info* _Type);

public:
	void AllDlgDestroy();
	void ActorSetting(TheOne* _pActor);
	void Reset();
};


