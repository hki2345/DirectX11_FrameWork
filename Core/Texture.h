#pragma once
#include "KResource.h"
#include "DirectXTex.h"
#include "DirectXHeader.h"





class Texture : public KResource
{
private:
	DirectX::ScratchImage			m_Image;
	ID3D11Texture2D*				m_Texture2D;

	// 텍스쳐에 넣어줄 세이더
	ID3D11ShaderResourceView*		m_View;

public:
	bool Load();

	float width() const { return (float)m_Image.GetMetadata().width; }
	float height() const { return (float)m_Image.GetMetadata().height; }
	KVector2 size() const{ return KVector2(width(), height()); }

public:
	void Update();

public:
	Texture();
	~Texture();
};

