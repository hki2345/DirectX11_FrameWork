#pragma once
#include "HRenderer.h"

class H3DGRID : public HRenderer
{
public:
	// KVector4.x는 BlockSize;
	// KVector4.y는 보더;
	// KVector4.z는 알파;
	// KVector4.w는 더미;

	float m_BlockSizeCur;
	float m_BlockSizeInter;
	// float m_BlockSizeMax;
	KVector4 m_GridData;

public:
	void TransUpdate(KPtr<HCamera> _Camera) override;
	void Render(KPtr<HCamera> _Camera) override;

public:
	bool Init(int _Order = 0) override;

private:
	void CalData(KPtr<HCamera> _Camera);

	void DebugRender() override;

public:
	H3DGRID();
	~H3DGRID();
};

