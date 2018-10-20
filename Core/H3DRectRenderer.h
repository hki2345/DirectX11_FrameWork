#pragma once
#include "HRenderer.h"

class H3DRectRenderer : public HRenderer
{
public:
	MatrixContainer m_MatData;

public:
	void Render(KPtr<HCamera> _Camera) override;

public:
	bool Init(int _Order = 0) override;
	// bool Init(const wchar_t* _ImgName, int _Order = 0);


public:
	H3DRectRenderer();
	~H3DRectRenderer();
};

