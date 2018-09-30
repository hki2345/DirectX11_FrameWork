// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "LightContainer.fx"

// Texture2D g_Tex_0 : register(t0);
// SamplerState g_Sam_0 : register(s0);

// 버텍스 버퍼에 대한 정보를 정의하는 
// POSITION을 시맨틱이라고 하는데.
// 버텍스 쉐이더에서 이용할 것.

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
LIGHT_VT_OUT VS_MESH3D(LIGHT_VT_IN _in)
{
	LIGHT_VT_OUT outData = (LIGHT_VT_OUT)0.f;

	// 픽셀에선 버텍스 계산이 아닌 픽셀 하나하나에 계싼이들어가야함
	outData.vPos = mul(_in.vPos, g_WVP);
	outData.vUv = _in.vUv;
	outData.vColor.rgba = _in.vColor.rgba;
	outData.vViewPos = mul(mul(_in.vPos, g_W), g_V);
	outData.vNormal = normalize(mul(mul(_in.vNormal, g_W), g_V));

	return outData;
}

LIGHT_PX_OUT PS_MESH3D(LIGHT_VT_OUT _in)
{
	LIGHT_PX_OUT outData = (LIGHT_PX_OUT)0.f;
	_in.vNormal.w = .0f;

	LightColor LC;
	if (0 == ArrList[0].Type)
	{
		LC = Direct_Light(_in.vViewPos, _in.vNormal, ArrList[0]);
	}
	else if (1 == ArrList[0].Type)
	{
		LC = Point_Light(_in.vViewPos, _in.vNormal, ArrList[0]);
	}





	// 픽셀 하나하나 산출될 때 이 계산을 하여 적용한다.
	outData.vColor.rgb = _in.vColor.rgb * LC.Diff.rgb + LC.Spec.rgb + LC.Ambi;
	outData.vColor.a = _in.vColor.a;

	return outData;
}