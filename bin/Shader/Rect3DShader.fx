#include "Matrix.fx"

// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.
struct Rect3D_VT_IN
{
	float4 vPos : POSITION;
	float2 vUv : UV;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
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
	// �׳� �����̱� ������ �������� �����Ų��.
	Rect3D_PX_OUT outData = (Rect3D_PX_OUT)0.0f;
	outData.vColor = _in.vColor;

	return outData;
}