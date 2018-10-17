#pragma once
#include "afxcmn.h"
#include <list>
#include <vector>
#include "ImageSelectButton.h"

// SpriteSelectDlg ��ȭ �����Դϴ�.

class SpriteSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SpriteSelectDlg)

public:
	SpriteSelectDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~SpriteSelectDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRITESELECT };
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

	std::vector<ImageSelectButton*> m_ButtonVector;

	CString ImageName;

public:
	void ButtonClick(UINT _Id);
};
