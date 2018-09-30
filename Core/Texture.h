#pragma once
#include "Resource.h"
#include "DirectXTex.h"
#include "DirectXHeader.h"





class Texture : public Resource
{
private:
	DirectX::ScratchImage			m_Image;
	ID3D11Texture2D*				m_Texture2D;

	// �ؽ��Ŀ� �־��� ���̴�
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

