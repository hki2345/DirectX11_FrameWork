#pragma once
#include "KResource.h"
#include "DirectXHeader.h"
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


class KFont : public KResource
{
private:
	IFW1Factory* m_Factory;
	IFW1FontWrapper* m_FontWrapper;

public:
	bool Create(const wchar_t* _Font);
	void draw_font(const wchar_t* _text, const KVector2& _Pos, const float& _Size, const UINT& _Color,
		FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);

public:
	KFont();
	~KFont();
};

