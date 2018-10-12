#include "MatrixContainer.fx"

Texture2D g_Tex_0 : register(t0);
SamplerState g_Sam_0 : register(s0);

struct DEBUG_VT_IN
{
	float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
};

struct DEBUG_VT_OUT
{
	float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
};

struct DEBUG_PX_OUT
{
	float4 vCol : SV_Target;
};




cbuffer Border : register(b3)
{
	float4 m_Color;
	float2 m_PixelPer;
	float2 m_Border;
}



DEBUG_VT_OUT VS_DebugRect(DEBUG_VT_IN _iN)
{
    DEBUG_VT_OUT outData = (DEBUG_VT_OUT) 0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;

	return outData;
}

DEBUG_PX_OUT PS_DebugRect(DEBUG_VT_OUT _in)
{    
    float4 vCol = g_Tex_0.Sample(g_Sam_0, _in.vUv);

    DEBUG_PX_OUT outData = (DEBUG_PX_OUT) 0.0f;
    outData.vCol = vCol;

    // 일단 ㅋㅋ
    return outData;
	
	// 1 == 사각형
	if (1 == m_Border.x)
	{
		if ((_in.vUv.x < m_PixelPer.x * m_Border.y || _in.vUv.x > 1.0f - (m_PixelPer.x * m_Border.y)) ||
			(_in.vUv.y < m_PixelPer.y * m_Border.y || _in.vUv.y > 1.0f - (m_PixelPer.y * m_Border.y)))
		{
			outData.vCol = m_Color;
		}
	}

	// 원
	else if (2 == m_Border.x)
	{
		if (_in.vUv.x * _in.vUv.x + _in.vUv.y * _in.vUv.y > (1.0f - m_PixelPer.x * m_Border.y) * (1.0f - m_PixelPer.x * m_Border.y))
		{
			outData.vCol = m_Color;
		}
	}

	if (0 >= outData.vCol.a)
	{
		outData.vCol = m_Color * .2f;
	}

	return outData;
}