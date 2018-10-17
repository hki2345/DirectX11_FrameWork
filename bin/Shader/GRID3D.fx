// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "GValue.fx"
// 버텍스 버퍼에 대한 정보를 정의하는 
// POSITION을 시맨틱이라고 하는데.
// 버텍스 쉐이더에서 이용할 것.
//struct VTX3DMESH_INPUT
//{
//	float4 vPos : POSITION;
//	float2 vUv : TEXCOORD;
//	float4 vColor : COLOR;
//	float4 vNormal : NORMAL;
//};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
struct VTX_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vWorldPos : POSTION;
};

struct PS_OUTPUT
{
	float4 vColor : SV_Target;
};

VTX_OUTPUT VS_GRID3D(float4 _Pos : POSITION)
{
	VTX_OUTPUT outData = (VTX_OUTPUT)0.f;

	outData.vPos = mul(_Pos, g_WVP);
	outData.vWorldPos = mul(_Pos, g_W);

	return outData;
}

cbuffer GRIDDATA : register(b0)
{
	float4 g_GridData;
}

PS_OUTPUT PS_GRID3D(VTX_OUTPUT _in)
{
	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
	// outData.vColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 y를 넘기는 방법은 비효율 적이다.
	// 이유 -> 픽셀 하나하나 마다 계산을 하게 될것이므로.

	// 칸 나누는 단위. 100.0f
	// 블록 나누는 단위
	// 100.0f 단위로

	float BlockSize = g_GridData.x;
	float Alpha = g_GridData.z;
	float Boader = g_GridData.y;


	float fX = abs(_in.vWorldPos.x % (BlockSize * 10));
	float fZ = abs(_in.vWorldPos.z % (BlockSize * 10));

	// 굵기 -> 카메라의 높이를 통해서 계산한다.

	// 그려져야 하는 애들만 빨간색으로 그리자.
	if (fZ <= Boader || fX <= Boader)
	{
		outData.vColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
		return outData;
	}

	fX = abs(_in.vWorldPos.x % (BlockSize));
	fZ = abs(_in.vWorldPos.z % (BlockSize));

	if (fZ <= Boader || fX <= Boader)
	{
		outData.vColor = float4(1.0f, 1.0f, 1.0f, Alpha);
		return outData;
	}

	clip(-1);

	return outData;
}