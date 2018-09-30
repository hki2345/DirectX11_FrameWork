#pragma once
#include "Renderer.h"
#include "KImage.h"

class Blend;
class Renderer_Sprite :public Renderer
{
private:
	KPtr<KImage>	m_Image;
	KColor			m_Color;
	KVector2		m_PixelPerUV;
	KVector2		m_Border;
	size_t			m_SplitIndex;

public:
	// 이미지를 실제 이미지 크기 그대로 세팅
	// 인자를 안 넣으면 이렇게 세팅해줌
	KVector2	set_image_size();
	KVector2	image_size();



	void shake_color(const KColor& _value) { m_Color = _value; }
	void image(const wchar_t* _Name);
	KPtr<KImage> image() { return m_Image; } 
	KPtr<Texture> texture() 
	{ 
		if (nullptr == m_Image)
		{
			return nullptr;
		}
		return m_Image->texture();
	}

	size_t& index()						{ return m_SplitIndex; }
	void	index(const size_t& _Value) { m_SplitIndex = _Value; }

	bool Init(int _Order = 0) override;
	bool Init(const wchar_t* _Name, int _Order = 0);
	void Render(KPtr<Camera> _Camera) override;


	KVector2&		border() { return m_Border; }
	void			border(const KVector2& _Value) { m_Border = _Value; }

public:
	Renderer_Sprite();
	~Renderer_Sprite();
};

