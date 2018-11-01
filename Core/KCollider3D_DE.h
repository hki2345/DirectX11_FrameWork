#pragma once
#include "KCollision.h"


class KMesh;
class KCollider3D_DE : public KCollision
{

protected:
	KPtr<KMesh> m_DegMesh;

public:
	virtual bool Init(const int& _Order = 0);

public:
	KCollider3D_DE();
	~KCollider3D_DE();
};

