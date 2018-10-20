#pragma once
#include "HRenderer.h"

class H3DGRID : public HRenderer
{
public:
	// KVector4.x�� BlockSize;
	// KVector4.y�� ����;
	// KVector4.z�� ����;
	// KVector4.w�� ����;

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

