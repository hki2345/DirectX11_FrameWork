#pragma once
#include "HRes.h"
#include "HVtxShader.h"
#include "HPixShader.h"
#include "HBlend.h"

// �����̶�� �ϴ� �༮�� ������̴�.
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

// � ���̴��� �ؽ�ó�� ����ϴ� �༮�ΰ�?
// �ؽ�ó�� ����ϸ� � �ؽ�ó�� ����ϴ°�?
class HTexture;
class HSampler;
class HMaterial : public HRes
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
	HPTR<HVtxShader> m_VtxShader;
	HPTR<HPixShader> m_PixShader;
	HPTR<HBlend> m_Blend;

public:
	HPTR<HVtxShader> VTXSH() {
		return m_VtxShader;
	}

	HPTR<HPixShader> PIXSH() {
		return m_PixShader;
	}

public:
	bool SetVtxShader(const wchar_t* _VtxName);
	bool SetPixShader(const wchar_t* _PixName);
	bool SetBlend(const wchar_t* _PixName);

public:
	bool Create();
	void Update();

	// �� �ؽ�ó�� ��� ������ ����ϴ���?
	// � �ؽ�ó��?

	// �� ���÷��� ��� ������ ����ϴ���?
	// � ���÷���?

	// ���� ���⿡ ���� ���̽� �ɼ��� �����ϴ� ���̴�.
private:
	std::vector<TexData> m_TexData;

public:
	void AddTexData(TEXTYPE _Type, UINT _TexSlot, const wchar_t* _TexName, UINT _SmpSlot = 0, const wchar_t* _SmpName = L"DefaultSmp");
	// void AddTexEffData();
	

private:
	std::unordered_map<unsigned int, HPTR<HTexture>>::iterator m_TexStartIter;
	std::unordered_map<unsigned int, HPTR<HTexture>>::iterator m_TexEndIter;
	std::unordered_map<unsigned int, HPTR<HTexture>> m_TexMap;

	std::unordered_map<unsigned int, HPTR<HSampler>>::iterator m_SmpStartIter;
	std::unordered_map<unsigned int, HPTR<HSampler>>::iterator m_SmpEndIter;
	std::unordered_map<unsigned int, HPTR<HSampler>> m_SmpMap;


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
	HPTR<HMaterial> Clone();


};
