#pragma once
#include "Resource.h"
#include "HVtxShader.h"
#include "HPixShader.h"
#include "HBlend.h"

// 재질이라고 하는 녀석을 만들것이다.
enum TEXTYPE 
{
	TT_COLOR,
	TT_BUMP,
	TT_TAGET,
};

class TexData
{
public:
	int Type;
	int Tex_Idx;
	int Tex_Smp;
	int Temp;

public:
	TexData() : Tex_Idx(-1), Tex_Smp(-1), Type(-1) {}
};

// 어떤 쉐이더가 텍스처를 사용하는 녀석인가?
// 텍스처를 사용하면 어떤 텍스처를 사용하는가?
class HTexture;
class HSampler;
class HMaterial : public Resource
{
	friend class HRenderer;
	friend class HLight;
	friend class HCamera;

public:
	HMaterial();
	~HMaterial();

private:
	HMaterial(const HMaterial& _Other);

private:
	bool IsOri;
	KPtr<HVtxShader> m_VtxShader;
	KPtr<HPixShader> m_PixShader;
	KPtr<HBlend> m_Blend;

public:
	KPtr<HVtxShader> VTXSH() {
		return m_VtxShader;
	}

	KPtr<HPixShader> PIXSH() {
		return m_PixShader;
	}

public:
	bool SetVtxShader(const wchar_t* _VtxName);
	bool SetPixShader(const wchar_t* _PixName);
	bool SetBlend(const wchar_t* _PixName);

public:
	bool Create();
	void Update();

	// 그 텍스처가 어느 슬롯을 사용하느냐?
	// 어떤 텍스처냐?

	// 그 샘플러가 어느 슬롯을 사용하느냐?
	// 어떤 샘플러냐?

	// 색깔 추출에 대한 베이스 옵션을 제공하는 것이다.
private:
	std::vector<TexData> m_TexData;

public:
	void AddTexData(TEXTYPE _Type, UINT _TexSlot, const wchar_t* _TexName, UINT _SmpSlot = 0, const wchar_t* _SmpName = L"DefaultSmp");
	// void AddTexEffData();
	

private:
	std::unordered_map<unsigned int, KPtr<HTexture>>::iterator m_TexStartIter;
	std::unordered_map<unsigned int, KPtr<HTexture>>::iterator m_TexEndIter;
	std::unordered_map<unsigned int, KPtr<HTexture>> m_TexMap;

	std::unordered_map<unsigned int, KPtr<HSampler>>::iterator m_SmpStartIter;
	std::unordered_map<unsigned int, KPtr<HSampler>>::iterator m_SmpEndIter;
	std::unordered_map<unsigned int, KPtr<HSampler>> m_SmpMap;


public:
	void SetTex(unsigned int _Slot, const wchar_t* _TexName);
	void SetTagetTex(unsigned int _Slot, const wchar_t* _TexName);
	void SetSmp(unsigned int _Slot, const wchar_t* _SmpName);
	unsigned int TexDataSetting(TexData* _Data);

private:
	void TexUpdate();
	void TexReset();
	void SmpUpdate();

public:
	KPtr<HMaterial> Clone();


};

