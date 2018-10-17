#include "HFont.h"
#include "HMACRO.h"
#include "HVAR.h"


HFont::HFont() : m_pFW1Factory(nullptr), m_pFontWrapper(nullptr)
{
}


HFont::~HFont()
{
	if (nullptr != m_pFW1Factory)
	{
		m_pFW1Factory->Release();
	}

	if (nullptr != m_pFontWrapper)
	{
		m_pFontWrapper->Release();
	}
}

bool HFont::Create(const wchar_t* _FontName)
{
	if (S_OK != FW1CreateFactory(FW1_VERSION, &m_pFW1Factory))
	{
		TASSERT(true);
		return false;
	}

	if (S_OK != m_pFW1Factory->CreateFontWrapper(HVAR::PDevice(), _FontName, &m_pFontWrapper))
	{
		TASSERT(true);
		return false;
	}
	return true;
}

void HFont::DrawFont(wchar_t* _pStr, HVEC2 _Pos, float _fSize, UINT _COLOR, FW1_TEXT_FLAG _Flag)
{
	m_pFontWrapper->DrawString(HVAR::Context(), _pStr, _fSize, _Pos.x, _Pos.y, _COLOR, _Flag);
}