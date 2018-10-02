// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "Matrix.fx"

Texture2D g_Tex_0 : register(t0);
SamplerState g_Sam_0 : register(s0);

struct IMG_VT_IN
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

struct IMG_VT_OUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

struct IMG_PX_OUT
{
	float4 vCol : SV_Target;
};


IMG_VT_OUT Img_VT(IMG_VT_IN _iN)
{
	IMG_VT_OUT outData = (IMG_VT_OUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;
	outData.vColor = _iN.vColor;
	outData.vNormal = _iN.vNormal;

	return outData;
}

cbuffer Border : register(b3)
{
	float4 CBColor;
	float2 m_PixelPer;
	float2 m_Border;
}

cbuffer ImageUv : register(b2)
{
	float2 vStart = float2(0.0f, 0.0f);
	float2 vSize = float2(1.0f, 1.0f);
};

IMG_PX_OUT Img_PX(IMG_VT_OUT _in)
{
	IMG_PX_OUT outData = (IMG_PX_OUT)0.0f;
	outData.vCol = g_Tex_0.Sample(g_Sam_0, float2(_in.vUv.x, _in.vUv.y));
	return outData;
}