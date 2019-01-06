#include "GValue.fx"
#include "PSOUT.fx"


cbuffer DEBUG_COLOR : register(b2)
{
    float4 vColor;
}

VS_COLOROUTPUT VS_DEBUG(VS_COLORINPUT _iN)
{
    VS_COLOROUTPUT outData = (VS_COLOROUTPUT) 0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vCol = _iN.vCol;

	return outData;
}

PS_OUTPUT PS_DEBUG(VS_COLOROUTPUT _in)
{
	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
    outData.vCol = _in.vCol * vColor;
	return outData;
}