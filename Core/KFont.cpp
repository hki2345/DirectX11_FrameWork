#include "KFont.h"
#include "KMacro.h"
#include "Core_Class.h"


KFont::KFont() : m_pFW1Factory(nullptr), m_pFontWrapper(nullptr)
{
}


KFont::~KFont()
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

bool KFont::Create(const wchar_t* _FontName)
{
	if (S_OK != FW1CreateFactory(FW1_VERSION, &m_pFW1Factory))
	{
		KASSERT(true);
		return false;
	}

	if (S_OK != m_pFW1Factory->CreateFontWrapper(Core_Class::PDevice(), _FontName, &m_pFontWrapper))
	{
		KASSERT(true);
		return false;
	}
	return true;
}

void KFont::Draw_Font(wchar_t* _pStr, KVector2 _Pos, float _fSize, UINT _COLOR, FW1_TEXT_FLAG _Flag)
{
	m_pFontWrapper->DrawString(Core_Class::Context(), _pStr, _fSize, _Pos.x, _Pos.y, _COLOR, _Flag);
}