#include "Matrix.fx"


// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
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
	// �׳� �����̱� ������ �������� �����Ų��.
	// ī�޶��� y�� �ѱ�� ����� ��ȿ�� ���̴�.
	// ���� -> �ȼ� �ϳ��ϳ� ���� ����� �ϰ� �ɰ��̹Ƿ�. -> �̰� ������Ʈ �������� ����Ѵٴ� ��


	float GridSize = g_GD.x;
	float Border = g_GD.y;
	float Alpha = g_GD.z;


	float fX = abs(_in.vWorldPos.x % (GridSize * 10.0f));
	float fZ = abs(_in.vWorldPos.z % (GridSize * 10.0f));

	// ���� -> ī�޶��� ���̸� ���ؼ� ����Ѵ�.

	// �׷����� �ϴ� �ֵ鸸 ���������� �׸���.
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