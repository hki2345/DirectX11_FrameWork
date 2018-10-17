#pragma once
#include "HRenderer.h"
#include "HImage.h"

class H3DMeshRenderer : public HRenderer
{
public:
	void Render(HPTR<HCamera> _Camera) override;

public:
	bool Init(int _Order = 0) override;

public:
	H3DMeshRenderer();
	~H3DMeshRenderer();
};

