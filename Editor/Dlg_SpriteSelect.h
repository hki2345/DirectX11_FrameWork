#pragma once
#include "afxcmn.h"
#include "Image_Selector.h"
#include <vector>


// Dlg_SpriteSelect 대화 상자입니다.

class Dlg_SpriteSelect : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_SpriteSelect)

public:
	Dlg_SpriteSelect(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_SpriteSelect();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SPRITESELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
