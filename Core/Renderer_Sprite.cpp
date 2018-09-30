#include "Renderer_Sprite.h"
#include "DirectXHeader_DE.h"
#include "KMacro.h"
#include "KWindow.h"
#include "ResourceManager.h"
#include "KShader.h"
#include "Blend.h"

#include "Texture.h"
#include "Sampler.h"


Renderer_Sprite::Renderer_Sprite() :
	m_Color(1.0f, 1.0f, 1.0f,1.0f),
	m_SplitIndex(0)
{
}


Renderer_Sprite::~Renderer_Sprite()
{
}

bool Renderer_Sprite::Init(int _Order)
{
	Renderer::Init(_Order);
	if (false == Set_Material(L"IMGMAT") || false == Set_Mesh(L"TEXMESH"))
	{
		return false;
	}

	return true;
}

bool Renderer_Sprite::Init(const wchar_t* _Name, int _Order)
{
	image(_Name);
	if (nullptr == m_Image)
	{
		return false;
	}

	if (false == Init(_Order))
	{
		return false;
	}

	return true;
}

void Renderer_Sprite::Render(KPtr<Camera> _Camera)
{
	if (nullptr == m_Image)
	{
		return;
	}

	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	if (false == one()->is_Active())
	{
		return;
	}
	
	// 부모인지 요놈인지 계산되어야 한다.
	Indepen_Update();

	if (nullptr != m_Image)
	{
		m_Image->sampler()->Update();
		m_Image->texture()->Update();
	}

	m_Border = 0.0f;
	m_Color = KColor::Green;

	// 넘기기전 보더세팅 - 스텍쓰기로 한다.
	KBorder NewBorder;
	NewBorder.m_Color = this->m_Color;
	NewBorder.m_UvPerPixel = KVector2(1.0f / m_Image->size().x, 1.0f / m_Image->size().y );
	NewBorder.m_Border = this->m_Border;



	// 디바이스로 정보 넘어가기 전에
	m_Material->VShader()->Set_ConstBuffer<KMatrix>(L"TRANS", 
		(independent_Matrix() * _Camera->View_Proj()).TransPose_Referance());
	m_Material->PShader()->Set_ConstBuffer<KBorder>(L"BORDER", NewBorder);
	m_Material->PShader()->Set_ConstBuffer<KVector4>(L"MULTI", m_Image->uv(m_SplitIndex));
	

	m_Material->Update();
	m_Mesh->Update();
	m_Mesh->Render();
}

void Renderer_Sprite::image(const wchar_t* _Name) 
{
	// 에딧에서 편집 편하게 하려고 일부러 null 을 넣어 m_Image를 비워줌
	if (nullptr == _Name)
	{
		m_Image = nullptr;
		return;
	}

	m_Image = ResourceManager<KImage>::Find(_Name);
	KASSERT(nullptr == m_Image);
}


KVector2 Renderer_Sprite::set_image_size()
{
	if (nullptr == m_Image )
	{
		return KVector2::Zero;
	}

	KVector2 NewScale = m_Image->texture()->size();
	inde_scale(NewScale);

	return NewScale;
}

KVector2 Renderer_Sprite::image_size()
{
	return m_Image->texture()->size();
}