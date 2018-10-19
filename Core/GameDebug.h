#pragma once
#include "DHeader.h"
#include "FW1FontWrapper.h"
#include <list>

class GameDebug
{
private:
	friend class KWindow;

private:
	class LogMsg
	{
	public:
		std::wstring Msg;
		HVEC m_COLOR;
	};

	static bool m_IsDebug;
	static float LogSize;
	static std::list<LogMsg>::iterator m_LogStart;
	static std::list<LogMsg>::iterator m_LogEnd;
	static std::list<LogMsg> m_Log;

	
public:
	static bool IsDebug() {
		return m_IsDebug;
	}

	static void DrawRect(const HRECT& _Rect, float _Border = 1);
	static void DrawRect(HVEC2 _Pos, HVEC2 _Size, float _Border = 1);
	static void DrawFont(wchar_t* _pStr, HVEC2 _Pos, float _fSize, HVEC _COLOR = HVEC::ONE, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);
	static void DrawLog(wchar_t* _pStr, HVEC _COLOR = HVEC::ONE);

public:
	static void RenderLog();
	static void TagetDebug();

public:
	GameDebug();
	~GameDebug();
};

