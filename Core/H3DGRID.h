#pragma once
#include "HRenderer.h"

class H3DGRID : public HRenderer
{
public:
	// HVEC.x�� BlockSize;
	// HVEC.y�� ����;
	// HVEC.z�� ����;
	// HVEC.w�� ����;

	float m_BlockSizeCur;
	float m_BlockSizeInter;
	// float m_BlockSizeMax;
	HVEC m_GridData;

public:
	void TransUpdate(HPTR<HCamera> _Camera) override;
	void Render(HPTR<HCamera> _Camera) override;

public:
	bool Init(int _Order = 0) override;

private:
	void CalData(HPTR<HCamera> _Camera);

	void DebugRender() override;

public:
	H3DGRID();
	~H3DGRID();
};

