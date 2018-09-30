#include "stdafx.h"
#include "Mouse_Droper.h"
#include "Editor.h"

BEGIN_MESSAGE_MAP(Mouse_Droper, CEdit)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

Mouse_Droper::Mouse_Droper() : m_Start(false)
{
}


Mouse_Droper::~Mouse_Droper()
{
}

afx_msg BOOL Mouse_Droper::OnSetCursor(CWnd* _pWnd, UINT _nHitTest, UINT _Msg)
{
	CEditorApp* App = (CEditorApp*)AfxGetApp();

	App->set_cursor(LoadCursor(nullptr, IDC_HAND));
	return TRUE;
}


void Mouse_Droper::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_Start = true;

	m_MS_CurX = point.x;
	m_Point = point;

	CEdit::OnLButtonDown(nFlags, point);
}


void Mouse_Droper::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_Start = false;

	CEdit::OnLButtonUp(nFlags, point);
}


void Mouse_Droper::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_Parent->UpdateData(TRUE);

	m_Point = point;

	if (true == m_Start)
	{
		int XValue = m_MS_CurX - point.x;

		if (nullptr != m_Value)
		{
			(*m_Value) -= (float)XValue;
		}

		if (nullptr != m_ValueChanger)
		{
			m_ValueChanger();
		}

		m_MS_CurX = point.x;
	}


	m_Parent->UpdateData(FALSE);


	CEdit::OnMouseMove(nFlags, point);
}
