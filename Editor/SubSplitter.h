#pragma once
class SubSplitter : public CSplitterWnd
{
private:
	bool m_bLock;

public:
	void lock() { m_bLock = true; }
	void unlock() { m_bLock = false; }

	int borderY() { return m_cyBorder; }

public:
	SubSplitter();
	~SubSplitter();

private:
	// 이 윈도우 즉,,, 해당 스필리터가 실행 되기전 초기화 단계 InitInstance생각하면 됌
	BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlages, CPoint point);
	afx_msg void OnMouseMove(UINT nFlages, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* _pWnd, UINT _nHitTest, UINT _Msg);
};

