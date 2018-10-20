#pragma once
#include "HRenderer.h"
#include "HImage.h"

class HBlend;
class HSpRenderer : public HRenderer
{
private:
	KPtr<HImage> m_Img;
	KColor	     m_Color;
	size_t		 m_ImgIndex;
	
public:
	void ImageScaleSetting();

	void Render(KPtr<HCamera> _Camera) override;

	void Color(const KColor& _Color)
	{
		m_Color = _Color;
	}

	size_t ImageIndex()
	{
		return m_ImgIndex;
	}
	KPtr<HImage> Image() { return m_Img; }

	void ImageIndex(size_t _Index) 
	{
		m_ImgIndex = _Index;
	}

	KPtr<HTexture> Texture() 
	{ 
		if (nullptr == m_Img)
		{
			return nullptr;
		}
		return m_Img->GetTex(); 
	}

	void Image(const wchar_t* _pImageName);

public:
	bool Init(int _Order = 0) override;
	bool Init(const wchar_t* _ImgName, int _Order = 0);

public:
	HSpRenderer();
	~HSpRenderer();
};

