#include "KImage.h"
#include "ResourceManager.h"
#include "KMacro.h"


KImage::KImage() :m_Texture(nullptr), m_Sampler(nullptr)
{
	m_UVvec.push_back({ .0f, .0f, 1.0f, 1.0f });
}


KImage::~KImage()
{
}


KPtr<Texture> KImage::texture() { return m_Texture; }
KPtr<Sampler> KImage::sampler() { return m_Sampler; }

bool KImage::Load(const bool& _Multi/* = false*/, const wchar_t* _SampleNa)
{
	// 텍스처 세팅
	m_Texture = ResourceManager<Texture>::Find(name_exist());
	if (m_Texture == nullptr)
	{
		m_Texture = ResourceManager<Texture>::Load(forder_path(), name_exist(), _Multi);
		KASSERT(nullptr == m_Texture);
	}

	m_Sampler = ResourceManager<Sampler>::Find(_SampleNa);
	if (m_Sampler == nullptr)
	{
		m_Sampler = ResourceManager<Sampler>::Create(_SampleNa);
		KASSERT(nullptr == m_Sampler);
	}

	return true;
}

size_t KImage::Split(const size_t& _X, const size_t& _Y)
{
	// 기본 방어 코드
	if (0 >= _X || 0 >= _Y) { KASSERT(true); }

	m_CutIdx.x = (float)_X;
	m_CutIdx.y = (float)_Y;

	// 자를 유브이를 초기화
	m_UVvec.clear();

	KVector4 Vec_Split;

	Vec_Split.z = 1.0f / (float)_X;
	Vec_Split.w = 1.0f / (float)_Y;

	for (size_t Y = 0; Y < _Y; Y++)
	{
		for (size_t X = 0; X < _X; X++)
		{
			Vec_Split.x = Vec_Split.z * X;
			Vec_Split.y = Vec_Split.w * Y;
			m_UVvec.push_back(Vec_Split);
		}
	}

	return m_UVvec.size() - 1;
}

size_t KImage::Split(const KVector4& _vec)
{
	m_UVvec.push_back(_vec);
	return m_UVvec.size() - 1;
}

bool KImage::Load(const size_t& _X, const size_t& _Y, const bool& _Multi/* = false*/, const wchar_t* _SampleNa/* = L"DefaultSam"*/)
{
	Load(_Multi, _SampleNa);
	Split(_X, _Y);
	return true;
}