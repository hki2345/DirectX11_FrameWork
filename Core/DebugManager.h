#pragma once
#include "DXContainer.h"
#include "FW1FontWrapper.h"
#include <list>

class DebugManager
{
public:
	DebugManager();
	~DebugManager();

private:
	friend class KWindow;

	static bool m_bDebug;
	static KColor m_LogColor;
	static float m_LogSize;
	static std::wstring	Messege;
	static std::list<std::wstring>::iterator m_S_Log;
	static std::list<std::wstring>::iterator m_E_Log;
	static std::list<std::wstring> m_LogList;

	
public:
	static bool Is_Debug() {
		return m_bDebug;
	}

	static void DrawRect(const KRect& _Rect, float _Border = 1);
	static void DrawRect(KVector2 _Pos, KVector2 _Size, float _Border = 1);
	static void DrawFont(wchar_t* _pStr, KVector2 _Pos, float _fSize, KVector4 _COLOR = KVector4::One, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);
	static void DrawLog(wchar_t* _pStr, KVector4 _COLOR = KVector4::One);

public:
	static void RenderLog();
	static void TagetDebug();

};

