#pragma once
#include "Resource.h"
#include "DirectXHeader.h"

#include "Texture.h"
#include "Sampler.h"

#include <vector>

class Texture;
class Sampler;
// 그냥 Image는 DirectXTex에 있어서 못씀
// 그래서 K를 붙임
class KImage : public Resource
{
private:
	KPtr<Texture> m_Texture;
	KPtr<Sampler> m_Sampler;
	std::vector<KVector4> m_UVvec;

	KVector2 m_CutIdx;


public:
	KVector2 size() const { return m_Texture->size(); }
	KVector2 index()
	{
		return m_CutIdx;
	}

	size_t split_cnt() const
	{
		return m_UVvec.size();
	}

	const KVector4& uv(size_t _Index = 0)
	{
		return m_UVvec[_Index];
	}

public:
	KPtr<Texture> texture();
	KPtr<Sampler> sampler();

public:
	size_t	Split(const size_t& _X, const size_t& _Y);

	// 벡터로 나누면 몇조각으로 나뉘었는지 정돈 알려준다.
	size_t	Split(const KVector4& _Vec);

	bool Load(const bool& _Multi = false, const wchar_t* _SampleNa = L"DefaultSam");
	bool Load(const size_t& _X, const size_t& _Y, const bool& _Multi = false, const wchar_t* _SampleNa = L"DefaultSam");

public:
	KImage();
	~KImage();
};

