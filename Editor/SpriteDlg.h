#pragma once
#include "afxcmn.h"
#include "TabDlg.h"
#include <HSpRenderer.h>
#include <list>
#include <map>
#include <GameDataStruct.h>

// SpriteDlg ��ȭ �����Դϴ�.

class SpriteDlg : public TabDlg
{
	DECLARE_DYNAMIC(SpriteDlg)

public:
	SpriteDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~SpriteDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRITEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnTvnSelchangedSpritetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeCutx();
	afx_msg void OnEnChangeCuty();

	void SpriteInit();
	void SpriteCheck(const wchar_t* _Path, const wchar_t* _Folder, HTREEITEM _ParentItem);

	KPtr<HSpRenderer> m_SpriteRender;

	std::map<std::wstring, SpriteData*> m_pDataList;

	CTreeCtrl m_SpriteTree;
	HTREEITEM m_CurSelectItem;
	CString m_SelectSpriteName;
	UINT m_CurX;
	UINT m_CurY;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSavedata();
	afx_msg void OnBnClickedLoaddata();

public:
	unsigned int TestPlayerLoad(void* _Arg);
	unsigned int TestPlayerLoad2(void* _Arg);

	void Release();
};
