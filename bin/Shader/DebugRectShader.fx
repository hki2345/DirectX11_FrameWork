// ���̴��� hlsl�̶�� �׷���ī�带 ���ؼ�
// �����ϵǴ� ���μ� �׷���ī�忡�� �̷��� ó���ش޶�� ����� ������ ���̴�.
//cbuffer TransformBuffer : register(b0)
//{
//	matrix g_W;
//	matrix g_V;
//	matrix g_P;
//}
#include "Matrix.fx"

// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.
struct VTXCOL_INPUT
{
	float4 vPos : POSITION;
	float2 vUv : UV;
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
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
	// �׳� �����̱� ������ �������� �����Ų��.
	PS_OUTPUT outData = (PS_OUTPUT)0.0f;
	
	// 1 == �簢��
	if (1 == m_Border.x)
	{
		if ((_in.vUv.x < m_PixelPer.x * m_Border.y || _in.vUv.x > 1.0f - (m_PixelPer.x * m_Border.y)) ||
			(_in.vUv.y < m_PixelPer.y * m_Border.y || _in.vUv.y > 1.0f - (m_PixelPer.y * m_Border.y)))
		{
			outData.vCol = m_Color;
		}
	}

	// ��
	else if (2 == m_Border.x)
	{
		if (_in.vUv.x * _in.vUv.x + _in.vUv.y * _in.vUv.y > (1.0f - m_PixelPer.x * m_Border.y) * (1.0f - m_PixelPer.x * m_Border.y))
		{
			outData.vCol = m_Color;
		}
	}
	// ����
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