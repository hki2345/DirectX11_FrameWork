#include "Matrix.fx"


// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
struct Grid3D_VT_OUT
{
	float4 vPos : SV_POSITION;
	float4 vWorldPos : POSITION;
};

struct Grid3D_PX_OUT
{
	float4 vColor : SV_Target;
};

Grid3D_VT_OUT Grid3D_VT(float4 _Pos : POSITION)
{
	Grid3D_VT_OUT outData = (Grid3D_VT_OUT)0.f;

	outData.vPos = mul(_Pos, g_WVP);
	outData.vWorldPos = mul(_Pos, g_W);

	return outData;
}

cbuffer GridData : register (b2)
{
	float4 g_GD;
	float4 g_Color;
}

Grid3D_PX_OUT Grid3D_PX(Grid3D_VT_OUT _in)
{
	Grid3D_PX_OUT OutData = (Grid3D_PX_OUT).0f;
	// 그냥 라인이기 때문에 직통으로 적용시킨다.
	// 카메라의 y를 넘기는 방법은 비효율 적이다.
	// 이유 -> 픽셀 하나하나 마다 계산을 하게 될것이므로. -> 이걸 업데이트 단위에서 계산한다는 뜻


	float GridSize = g_GD.x;
	float Border = g_GD.y;
	float Alpha = g_GD.z;


	float fX = abs(_in.vWorldPos.x % (GridSize * 10.0f));
	float fZ = abs(_in.vWorldPos.z % (GridSize * 10.0f));

	// 굵기 -> 카메라의 높이를 통해서 계산한다.

	// 그려져야 하는 애들만 빨간색으로 그리자.
	if (fZ <= Border || fX <= Border)
	{
		OutData.vColor = float4(g_Color.x, g_Color.y, g_Color.z, 1.0f);
		return OutData;
	}

	fX = abs(_in.vWorldPos.x % (GridSize));
	fZ = abs(_in.vWorldPos.z % (GridSize));

	if (fZ <= Border || fX <= Border)
	{
		OutData.vColor = float4(g_Color.x * Alpha, g_Color.y* Alpha, g_Color.z* Alpha, Alpha);
		return OutData;
	}

	clip(-1);
	return OutData;
}