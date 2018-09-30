#pragma once
#include "afxcmn.h"
#include "TabState.h"
#include <Renderer_Sprite.h>

#include <GameData.h>
#include <map>

// Dlg_Sprite ��ȭ �����Դϴ�.
class Dlg_SpriteTab : public TabState
{
	DECLARE_DYNAMIC(Dlg_SpriteTab)

public:
	Dlg_SpriteTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_SpriteTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRITE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeSpriteu();
	afx_msg void OnEnChangeSpritev();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedSave();

	
	void Init();
	void check(const wchar_t* _Path, const wchar_t* _Folder, HTREEITEM _ParentItem);
	void Release();

	KPtr<Renderer_Sprite> m_SpRenderer;
	std::list<Renderer_Sprite*> m_SpriteList;
	
	std::map<std::wstring, Split_Data*> m_SplitData_Map;

	HTREEITEM m_CurItem;
	UINT m_SplitX;
	UINT m_SplitY;

	CTreeCtrl m_Sprite_Tree;
	CString m_SpriteName;

};
