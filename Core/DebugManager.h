#pragma once
#include "DirectXHeader.h"
#include "FW1FontWrapper.h"
#include <list>

#define DRAW_FONT DebugManager::draw_font
#define WLOG		DebugManager::WLog

class TransPosition;
class TheOne;
// 어떠한 것을 관리하는게 아니라 어떤 객체에서 디버깅 랜더를 실행하게 한다.
class DebugManager
{
private:
	friend class KWindow;

private:
	static KColor m_LogColor;
	static float m_LogSize;
	static std::wstring	Messege;
	static std::list<std::wstring>::iterator m_S_Log;
	static std::list<std::wstring>::iterator m_E_Log;
	static std::list<std::wstring> m_LogList;


private:
	static bool m_Debug;

public:
	static void Log_Color(const KColor& _Value)
	{
		m_LogColor = _Value;
	}

	static void Debug_On()
	{
		m_Debug = true;
	}
	static void Debug_Off()
	{
		m_Debug = false;
	}
	static bool Is_Debug()
	{
		return m_Debug;
	}

	static void draw_rect(const KRect& _Rect, const KColor& _Color, const float& _Border = 1);
	static void draw_rect(const KVector2& _Pos, const KVector2& _Size, const KColor& _Color = { .0f, 1.0f, .0f, 1.0f }, const float& _Border = 1);
	static void draw_font(const wchar_t* _pStr, const KVector2& _Pos, const float& _fSize, const KColor& _COLOR = KColor::Black, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);
	static void WLog(const wchar_t* const _Str, ...);

public:
	static void Logging();

public:
	DebugManager();
	~DebugManager();
};

