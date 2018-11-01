#pragma once
#include "KCollision.h"
class KCollider3D_DE : public KCollision
{
public:
	bool Init(const int& _Order = 0);

public:
	KCollider3D_DE();
	~KCollider3D_DE();
};

