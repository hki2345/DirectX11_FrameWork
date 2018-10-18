#pragma once
#include "HRes.h"
#include "DHeader.h"
#include "HTexture.h"
#include "HSampler.h"

#include <vector>

class HTexture;
class HSampler;
class HImage : public HRes
{
private:
	KPtr<HTexture> m_Tex;
	KPtr<HSampler> m_Sam;
	std::vector<HVEC> m_VecUv;

	HVEC2 m_CutIndex;

public:
	HVEC2 CutIndex() {
		return m_CutIndex;
	}

	size_t CutCount() {
		return m_VecUv.size();
	}

	const HVEC& Uv(size_t _Index = 0) 
	{
		return m_VecUv[_Index];
	}


public:
	KPtr<HTexture> GetTex();
	KPtr<HSampler> GetSam();

public:
	// 이미지를 자르는 함수입니다.
	void Cut(size_t _X, size_t _Y);
	size_t Cut(HVEC _Vec);

public:
	bool Load(const wchar_t* _SmpName = L"DefaultSmp");
	bool Load(int _CutX, int _CutY, const wchar_t* _SmpName = L"DefaultSmp");

public:
	HImage();
	~HImage();
};

