#pragma once
#include <functional>

class Mouse_Droper : public CEdit
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnSetCursor(CWnd* _pWnd, UINT _nHitTest, UINT _Msg);


public:
	bool m_Start;

	CPoint m_Point;
	float* m_Value;


	CDialogEx* m_Parent;
	int m_MS_CurX;
	std::function<void()> m_ValueChanger;

public:
	template<typename T>
	void Drop_ValueChager(T* _Obj, void(T::*_PTR)())
	{
		if (nullptr == _PTR) { return; }
		if (nullptr == _Obj) { return; }
		m_ValueChanger = std::bind(_PTR, _Obj);
	}

public:
	Mouse_Droper();
	~Mouse_Droper();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

