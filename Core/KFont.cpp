#include "KFont.h"
#include "KMacro.h"
#include "Core_Class.h"
#include "KDevice.h"


KFont::KFont(): m_Factory (nullptr), m_FontWrapper(nullptr)
{
}


KFont::~KFont()
{
	if (nullptr != m_Factory)
	{
		m_Factory->Release();
	}

	if (nullptr != m_FontWrapper)
	{
		m_FontWrapper->Release();
	}
}


bool KFont::Create(const wchar_t* _Font)
{
	if (S_OK != FW1CreateFactory(FW1_VERSION, &m_Factory))
	{
		KASSERT(true);
		return false;
	}

	if (S_OK != m_Factory->CreateFontWrapper(Core_Class::device(), _Font, &m_FontWrapper))
	{
		KASSERT(true);
		return false;
	}
	return true;
}


void KFont::draw_font(const wchar_t* _text, const KVector2& _Pos, const float& _Size,
	const UINT& _Color, FW1_TEXT_FLAG _Flag/* = FW1_TEXT_FLAG::FW1_TOP*/)
{
	m_FontWrapper->DrawString(Core_Class::device_context(), _text, _Size, _Pos.x, _Pos.y, _Color, _Flag);
	Core_Class::MainDevice().reset_context();
}