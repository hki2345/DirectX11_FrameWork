
// AR14TOOL.h : AR14TOOL ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CAR14TOOLApp:
// �� Ŭ������ ������ ���ؼ��� AR14TOOL.cpp�� �����Ͻʽÿ�.
//

class CAR14TOOLApp : public CWinApp
{
public:
	CAR14TOOLApp();

public:
	void MyCurSor(HCURSOR _hCur);

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CAR14TOOLApp theApp;
