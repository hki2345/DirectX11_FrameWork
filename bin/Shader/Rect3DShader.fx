#include "Matrix.fx"

// 버텍스 버퍼에 대한 정보를 정의하는 
// POSITION을 시맨틱이라고 하는데.
// 버텍스 쉐이더에서 이용할 것.
struct Rect3D_VT_IN
{
	float4 vPos : POSITION;
	float2 vUv : UV;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
struct Rect3D_VT_OUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : UV;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

struct Rect3D_PX_OUT
{
	float4 vColor : SV_Target;
};

Rect3D_VT_OUT Rect3D_VT(Rect3D_VT_IN _iN)
{
	Rect3D_VT_OUT outData = (Rect3D_VT_OUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;
	outData.vColor = _iN.vColor;
	outData.vNormal = _iN.vNormal;

	return outData;
}

Rect3D_PX_OUT Rect3D_PX(Rect3D_VT_OUT _in)
{
	// 그냥 라인이기 때문에 직통으로 적용시킨다.
	Rect3D_PX_OUT outData = (Rect3D_PX_OUT)0.0f;
	outData.vColor = _in.vColor;

	return outData;
}