#pragma once
#include "HShader.h"
#include <vector>

class HVtxShader : public HShader
{
private:
	ID3D11VertexShader* m_pShader;

	std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputLayoutData;
	UINT m_Offset;
	ID3D11InputLayout*				      m_pInputLayout;

public:
	// 버전 HLSL 컴파일러 버전을 의미한다.
	bool Load(const char* _FuncName, UINT _VH = 5, UINT _VL = 0);
	void Update() override;

	//LPCSTR SemanticName;
	//UINT SemanticIndex;
	//DXGI_FORMAT Format;
	//UINT InputSlot;
	//UINT AlignedByteOffset;
	//D3D11_INPUT_CLASSIFICATION InputSlotClass;
	//UINT InstanceDataStepRate;
	bool AddLayout(LPCSTR _SName, UINT _SI, DXGI_FORMAT _Fm, UINT _IS, D3D11_INPUT_CLASSIFICATION _ISC = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, UINT _IDSR = 0);
	bool AddLayoutEnd(LPCSTR _SName, UINT _SI, DXGI_FORMAT _Fm, UINT _IS, D3D11_INPUT_CLASSIFICATION _ISC = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, UINT _IDSR = 0);
	bool CreateLayout();
	void SetLayout();

public:
	void UpdateCB(KPtr<CBUFFER> _Buf);

public:
	HVtxShader();
	~HVtxShader();
};

