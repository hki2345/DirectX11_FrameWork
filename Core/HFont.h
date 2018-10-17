#pragma once
#include "HRes.h"
#include "DHeader.h"
#include "FW1CompileSettings.h"
#include "FW1FontWrapper.h"

#if X64
#if _DEBUG
#pragma comment(lib, "FW1FontWrapper_X64_DEBUG")
#else if NDEBUG
#pragma comment(lib, "FW1FontWrapper_X64_RELEASE")
#endif
#else if WIN32
#if _DEBUG
#pragma comment(lib, "FW1FontWrapper_X86_DEBUG")
#else if NDEBUG
#pragma comment(lib, "FW1FontWrapper_X86_RELEASE")
#endif
#endif // DEBUG

class HFont : public HRes
{
private:
	IFW1Factory* m_pFW1Factory;
	IFW1FontWrapper* m_pFontWrapper;

public:
	bool Create(const wchar_t* _FontName);
	void DrawFont(wchar_t* _pStr, HVEC2 _Pos, float _fSize, UINT _COLOR, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);

public:
	HFont();
	~HFont();
};
