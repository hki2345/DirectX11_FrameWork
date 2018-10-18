#pragma once
#include "HRenderer.h"

class H3DGRID : public HRenderer
{
public:
	// HVEC.x는 BlockSize;
	// HVEC.y는 보더;
	// HVEC.z는 알파;
	// HVEC.w는 더미;

	float m_BlockSizeCur;
	float m_BlockSizeInter;
	// float m_BlockSizeMax;
	HVEC m_GridData;

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

