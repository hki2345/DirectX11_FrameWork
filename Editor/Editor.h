
// Editor.h : Editor ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CEditorApp:
// �� Ŭ������ ������ ���ؼ��� Editor.cpp�� �����Ͻʽÿ�.
//

class CEditorApp : public CWinApp
{
public:
	CEditorApp();

public:
	void set_cursor(HCURSOR _hCur);


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	// mfc���� loop�� ���� ����
	virtual BOOL OnIdle(LONG lCount);
};

extern CEditorApp theApp;
