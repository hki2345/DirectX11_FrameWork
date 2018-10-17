#pragma once
#include "afxcmn.h"
#include <list>
#include <vector>
#include "ImageSelectButton.h"

// SpriteSelectDlg 대화 상자입니다.

class SpriteSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SpriteSelectDlg)

public:
	SpriteSelectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~SpriteSelectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRITESELECT };
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

	std::vector<ImageSelectButton*> m_ButtonVector;

	CString ImageName;

public:
	void ButtonClick(UINT _Id);
};
