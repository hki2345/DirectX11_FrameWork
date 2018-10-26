#pragma once
#include "Resource.h"
#include "Shader_Vertex.h"
#include "Shader_Pixel.h"
#include "KBlend.h"

// 재질이라고 하는 녀석을 만들것이다.
enum TEX_TYPE 
{
	TEX_COLOR,
	TEX_BUMP,
	TEX_SPEC,
	TEX_TAGET,
};

class Tex_Data
{
public:
	int Type;
	int Tex_Idx;
	int Tex_Smp;
	int Temp;

public:
	Tex_Data() : Tex_Idx(-1), Tex_Smp(-1), Type(-1) {}
};





// 어떤 쉐이더가 텍스처를 사용하는 녀석인가?
// 텍스처를 사용하면 어떤 텍스처를 사용하는가?
class Texture;
class Sampler;
class KMaterial : public Resource
{
	friend class Renderer;
	friend class Light;
	friend class Camera;

public:
	KMaterial();
	~KMaterial();

private:
	KMaterial(const KMaterial& _Other);

private:
	bool m_bOrigin;
	KPtr<Shader_Vertex> m_VTS;
	KPtr<Shader_Pixel> m_PXS;
	KPtr<KBlend> m_Blend;

public:
	KPtr<Shader_Vertex> VShader() {
		return m_VTS;
	}

	KPtr<Shader_Pixel> PShader() {
		return m_PXS;
	}

public:
	bool Set_VTShader(const wchar_t* _VtxName);
	bool Set_PXShader(const wchar_t* _PixName);
	bool Set_Blend(const wchar_t* _PixName);

public:
	bool Create();
	void Update();

	// 그 텍스처가 어느 슬롯을 사용하느냐?
	// 어떤 텍스처냐?

	// 그 샘플러가 어느 슬롯을 사용하느냐?
	// 어떤 샘플러냐?

	// 색깔 추출에 대한 베이스 옵션을 제공하는 것이다.
private:
	std::vector<Tex_Data> m_TDVec;

public:
	void Insert_TexData(TEX_TYPE _Type, const KUINT& _TexSlot, const wchar_t* _TexName, UINT _SmpSlot = 0, const wchar_t* _SmpName = L"DefaultSmp");

private:
	std::unordered_map<unsigned int, KPtr<Texture>>::iterator m_TSI;
	std::unordered_map<unsigned int, KPtr<Texture>>::iterator m_TEI;
	std::unordered_map<unsigned int, KPtr<Texture>> m_TexMap;

	std::unordered_map<unsigned int, KPtr<Sampler>>::iterator m_SSI;
	std::unordered_map<unsigned int, KPtr<Sampler>>::iterator m_SEI;
	std::unordered_map<unsigned int, KPtr<Sampler>> m_SamMap;


public:
	void Set_Tex(const KUINT& _Slot, const wchar_t* _TexName);
	void Set_Sampler(const KUINT& _Slot, const wchar_t* _SmpName);
	void Set_TargetTex(const KUINT& _Slot, const wchar_t* _TexName);
	KUINT Set_TexData(Tex_Data* _Data);

private:
	void Update_Tex();
	void Update_Sam();


	// 타겟으로 하면 리셋이 필요
	void Reset_Tex();

public:
	KPtr<KMaterial> Clone();
};

