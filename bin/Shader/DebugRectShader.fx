// 쉐이더는 hlsl이라는 그래픽카드를 통해서
// 컴파일되는 언어로서 그래픽카드에게 이렇게 처리해달라고 명령을 내리는 것이다.
//cbuffer TransformBuffer : register(b0)
//{
//	matrix g_W;
//	matrix g_V;
//	matrix g_P;
//}
#include "Matrix.fx"

// 버텍스 버퍼에 대한 정보를 정의하는 
// POSITION을 시맨틱이라고 하는데.
// 버텍스 쉐이더에서 이용할 것.
struct VTXCOL_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : UV;
};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
struct VTXCOL_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : UV;
};

struct PS_OUTPUT
{
	float4 vCol : SV_Target;
};




cbuffer Border : register(b3)
{
	float4 m_Color;
	float2 m_PixelPer;
	float2 m_Border;
}

cbuffer Poly : register(b4)
{
	float4 m_Vertex;
}


VTXCOL_OUTPUT VS_DebugPoly(VTXCOL_INPUT _iN)
{
	VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT)0.f;

	outData.vPos = _iN.vPos;
	outData.vUv = _iN.vUv;

	return outData;
}

VTXCOL_OUTPUT VS_DebugRect(VTXCOL_INPUT _iN)  
{
	VTXCOL_OUTPUT outData = (VTXCOL_OUTPUT)0.f;

	outData.vPos = mul(_iN.vPos, g_WVP);
	outData.vUv = _iN.vUv;

	return outData;
}

PS_OUTPUT PS_DebugRect(VTXCOL_OUTPUT _in)
{
	// 그냥 라인이기 때문에 직통으로 적용시킨다.
	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
	
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
	// 폴리
	else if (3 == m_Border.x)
	{
		outData.vCol = m_Color * .2f;
		return outData;
	}
	

	if (0 >= outData.vCol.a)
	{
		outData.vCol = m_Color * .2f;
	}

	return outData;
}