#pragma once
#include <TheOne.h>
#include <list>

#include "ComDlg.h"

// ComView ���Դϴ�.

class ComView : public CScrollView
{
	DECLARE_DYNCREATE(ComView)

protected:
	ComView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~ComView();

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


