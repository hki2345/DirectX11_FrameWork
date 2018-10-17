// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "GValue.fx"
// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.
//struct VTX3DMESH_INPUT
//{
//	float4 vPos : POSITION;
//	float2 vUv : TEXCOORD;
//	float4 vColor : COLOR;
//	float4 vNormal : NORMAL;
//};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
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

	// ī�޶��� y�� �ѱ�� ����� ��ȿ�� ���̴�.
	// ���� -> �ȼ� �ϳ��ϳ� ���� ����� �ϰ� �ɰ��̹Ƿ�.

	// ĭ ������ ����. 100.0f
	// ��� ������ ����
	// 100.0f ������

	float BlockSize = g_GridData.x;
	float Alpha = g_GridData.z;
	float Boader = g_GridData.y;


	float fX = abs(_in.vWorldPos.x % (BlockSize * 10));
	float fZ = abs(_in.vWorldPos.z % (BlockSize * 10));

	// ���� -> ī�޶��� ���̸� ���ؼ� ����Ѵ�.

	// �׷����� �ϴ� �ֵ鸸 ���������� �׸���.
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