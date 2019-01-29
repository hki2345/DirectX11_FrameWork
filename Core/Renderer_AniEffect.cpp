#include "Renderer_AniEffect.h"
#include "Texture.h"
#include "Core_Class.h"
#include "TimeManager.h"


Renderer_AniEffect::Renderer_AniEffect()
{
}


Renderer_AniEffect::~Renderer_AniEffect()
{
}

void Renderer_AniEffect::EffectSetting(const wchar_t* _TexName, int _X, int _Y, bool _Loop, float _InterTime /*= 0.1f*/)
{
	m_Mtl->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, _TexName);
	X = _X;
	Y = _Y;
	CURX = 0;
	CURY = 0;
	m_InterTime = _InterTime;

	m_Data.m_Size.x = 1.0f / (float)X;
	m_Data.m_Size.y = 1.0f / (float)Y;
	m_Data.m_Color = KVector4::One;

}



void Renderer_AniEffect::Update_Trans(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	SubTransUpdate();
	
	// ������
	m_MD.m_W = m_Trans->worldmat_const() * _Camera->Trans()->rotatemat_const();
	m_MD.m_V = _Camera->View();
	m_MD.m_P = _Camera->Proj();
	m_MD.m_WV = (m_MD.m_W * _Camera->View());
	m_MD.m_VP = (_Camera->ViewProj());
	m_MD.m_WVP = (m_MD.m_W* _Camera->ViewProj());
	m_MD.m_CamPos = _Camera->Trans()->pos_world();
	m_MD.RTrans();
}

void Renderer_AniEffect::Update()
{
	// �ӷ��̶� �̷��� ���õȰɷ� �ִϸ��̼� �����ִ°�.
	m_EffectTime -= DELTATIME;

	if (0.0f >= m_EffectTime)
	{
		++CURX;
		m_EffectTime = m_InterTime;

		if (CURX >= X)
		{
			CURX = 0;
			++CURY;

			if (CURY >= Y)
			{
				CURY = 0;
			}
		}
	}

	m_Data.m_Start.x = m_Data.m_Size.x * CURX;
	m_Data.m_Start.y = m_Data.m_Size.y * CURY;
}


bool Renderer_AniEffect::Init(const int& _Order/* = 0*/)
{
	Renderer::Init(_Order);

	Set_Mesh(L"RECT");
	Set_Material(L"ANIEFFMTL");
	m_Mtl = material(0);

	ROpt.Defferd_orForward = 0;
	ROpt.Render_DT = RENDER_DATATYPE::RDT_BASE;

	return true;
}

// �ش� ȿ�� ������ ������ ���� ������ ���� �κ��̴�.
void Renderer_AniEffect::Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data)
{
	std::wstring PrevDs = Core_Class::MainDevice().ds_name();

	Core_Class::MainDevice().Set_DSS(L"ALWAYS");

	m_Mtl->PShader()->SettingCB<ANIEFFCB>(L"ANIEFFCB", m_Data);

	// cb���� ���ְ�.
	Update_TexSmp(0);
	Update_Material(0);
	Update_Mesh(0);

	Core_Class::MainDevice().Set_DSS(PrevDs.c_str());
}
