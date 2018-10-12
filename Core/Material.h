#pragma once
#include "KResource.h"
#include "Vertex_Shader.h"
#include "Pixel_Shader.h"
#include "Blend.h"

enum Texture_Type
{
	TEX_COLOR,
	TEX_BUMP,
	TEX_TARGET,
};

class Texture_Data
{

public:
	int TInx;
	int TSmp;
	int ESmp;
	int Type;

	// -1이 없는 걸로 초기화
public:
	Texture_Data() : TInx(-1), TSmp(-1),/* EInx(-1), */ESmp(-1), Type(-1){}
};


class Texture;
class Sampler;
class Material : public KResource
{

public:
	Material();
	Material(const Material& _Other);
	~Material();

private:
	friend class Renderer;
	friend class KLight;
	friend class Camera;

public:
	bool Check_Original;
	KPtr<Vertex_Shader> m_Vertex_Shader;
	KPtr<Pixel_Shader> m_Pixel_Shader;
	KPtr<Blend>			m_Blend;

public:
	KPtr<Vertex_Shader> VShader()
	{
		return m_Vertex_Shader;
	}
	KPtr<Pixel_Shader> PShader()
	{
		return m_Pixel_Shader;
	}

	bool Set_VShader(const wchar_t* _Name);
	bool Set_PShader(const wchar_t* _Name);
	bool Set_Blend(const wchar_t* _Name);


	bool Create();
	void Update();

private:
	std::vector<Texture_Data> m_TD_Vec;

public:
	void insert_TD(const Texture_Type& _Type, UINT _TexSlot, const wchar_t* _TexName, const UINT& _SmSlot = 0,
		const wchar_t* _SmName = L"DefaultSam");
	KPtr<Material> Clone()
	{
		return new Material(*this);
	}

private:
	std::unordered_map<unsigned int, KPtr<Texture>>::iterator m_TSI;
	std::unordered_map<unsigned int, KPtr<Texture>>::iterator m_TEI;
	std::unordered_map<unsigned int, KPtr<Texture>> m_TexMap;


	std::unordered_map<unsigned int, KPtr<Sampler>>::iterator m_SSI;
	std::unordered_map<unsigned int, KPtr<Sampler>>::iterator m_SEI;
	std::unordered_map<unsigned int, KPtr<Sampler>> m_SamMap;

public:
	void texture(const KUINT& _Slot, const wchar_t* _Name);
	void texture_target(const KUINT& _Slot, const wchar_t* _Name);
	void sampler(const UINT& _Slot, const wchar_t* _Name);
	UINT texture_data(Texture_Data* _Data);

private:
	void Update_Tex();
	void Update_Sam();

	// 타겟으로 하면 리셋이 필요
	void Reset();

};

