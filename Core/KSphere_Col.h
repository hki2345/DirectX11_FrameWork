#pragma once
#include "KCollision.h"


class KSphere_Col : public KCollision
{
public:
	KSphereCon* pSphere;

public:
	void ColFiUpdate() override;
	void DebugRender() override;

public:
	KSphere_Col();
	~KSphere_Col();
};

