#pragma once
#include "HShader.h"
class HPixShader : public HShader
{
private:
	ID3D11PixelShader* m_pShader;

public:
	bool Load(const char* _FuncName, UINT _VH = 5, UINT _VL = 0);
	void Update() override;

private:
	void UpdateCB(HPTR<CBUFFER> _Buf);

public:
	HPixShader();
	~HPixShader();
};

