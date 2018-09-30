#pragma once
#include "afxcmn.h"
#include "Image_Selector.h"
#include <vector>


// Dlg_SpriteSelect ��ȭ �����Դϴ�.

class Dlg_SpriteSelect : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_SpriteSelect)

public:
	Dlg_SpriteSelect(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_SpriteSelect();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SPRITESELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	SIZE m_SampleSize;
	SIZE m_SlotSize;
	UINT CountX;

	std::vector<Image_Selector*> m_ImageBtn_Vec;

	CString Image_Path;
	CString Image_Name;

public:
	void ButtonClick(UINT _Id);
};
