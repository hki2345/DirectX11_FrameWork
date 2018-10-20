#include "HImage.h"
#include "HTexture.h"
#include "HSampler.h"
#include "HResMgr.h"
#include "KMacro.h"


HImage::HImage() 
{
	m_VecUv.push_back({ 0.0f, 0.0f, 1.0f, 1.0f });
}

HImage::~HImage()
{
}

KPtr<HTexture> HImage::GetTex() {
	return m_Tex;
}

KPtr<HSampler> HImage::GetSam() {
	return m_Sam;
}

bool HImage::Load(const wchar_t* _SmpName /*= L"DefaultSmp"*/)
{
	m_Tex = ResourceManager<HTexture>::Find(FileNameExt());

	if (nullptr == m_Tex)
	{
		m_Tex = ResourceManager<HTexture>::Load(FileForder(), FileNameExt());
		KASSERT(nullptr == m_Tex);
	}

	m_Sam = ResourceManager<HSampler>::Find(_SmpName);

	if (nullptr == m_Sam)
	{
		m_Sam = ResourceManager<HSampler>::Create(_SmpName);
		KASSERT(nullptr == m_Tex);
	}

	return true;
}


bool HImage::Load(int _CutX, int _CutY, const wchar_t* _SmpName /*= L"DefaultSmp"*/)
{
	Load(_SmpName);
	Cut(_CutX, _CutY);
	return true;
}

void HImage::Cut(size_t _X, size_t _Y)
{
	if (0 >= _X)
	{
		KASSERT(0 >= _X);
	}

	if (0 >= _Y)
	{
		KASSERT(0 >= _Y);
	}

	m_CutIndex.x = (float)_X;
	m_CutIndex.y = (float)_Y;

	// 0,0,1,1 ³¯·È´Ù.
	m_VecUv.clear();

	KVector4 CutVec;

	CutVec.z = 1.0f / (float)_X;
	CutVec.w = 1.0f / (float)_Y;

	for (size_t Y = 0; Y < (size_t)_Y; ++Y)
	{
		for (size_t X = 0; X < (size_t)_X; ++X)
		{
			CutVec.x = CutVec.z * X;
			CutVec.y = CutVec.w * Y;
			m_VecUv.push_back(CutVec);
		}
	}
}

size_t HImage::Cut(KVector4 _Vec) 
{
	m_VecUv.push_back(_Vec);
	return m_VecUv.size() - 1;
}