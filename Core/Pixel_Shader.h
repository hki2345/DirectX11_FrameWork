#pragma once
#include "KShader.h"

// 픽셀과 버텍스는 따로 분리되어 있는데
// 나중에 렌더 할떄 나눠서 하라고
class Pixel_Shader :	public KShader
{
private:
	ID3D11PixelShader* m_Shader;

public:
	// 함수 이름 -> char로 받는다. GPU에서 \
	// 뒤에 두개의 UINT는 버젼 정보
	bool Load(const char* _Func, UINT _VH = 5, UINT _VL = 0);
	void Update() override;


private:
	void UpdateCB(KPtr<Const_Buffer> _Buf) override;

public:
	Pixel_Shader();
	~Pixel_Shader();
};

