#include "Matrix.fx"

class LightColor
{
	float4 Diff; // Ȯ�걤
	float4 Spec; // �ݻ籤
	float4 Ambi; // ȯ�汤
};
class LightData
{
	LightColor Lc;
	float4 Pos;
	float4 Dir;
	float4 CamPos;
	float Range;
	float Angle;
	int Type;
};

cbuffer LightBuffer : register(b10)
{
	LightData ArrList[10];
	int       LightCount;
	int       Temp1;
	int       Temp2;
}


struct LIGHT_VT_IN
{
	float4 vPos : POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vNormal : NORMAL;
};

struct LIGHT_VT_OUT
{
	float4 vPos : SV_POSITION;
	float2 vUv : TEXCOORD;
	float4 vColor : COLOR;
	float4 vViewPos : POSITION;
	float4 vNormal : NORMAL;
};

struct LIGHT_PX_OUT
{
	float4 vColor : SV_Target;
};


LightColor Direct_Light(float4 _vViewPos, float4 _vViewNormal, LightData _Info)
{
	LightColor CALL = (LightColor)0.f;

	// ����Ʈ�� ���� �ȉ´� -> �̰� ���̷�Ʈ -> ������ �����̴�
	float4 vLight = _Info.Dir;
	vLight = normalize(vLight);
	CALL.Diff = _Info.Lc.Diff * saturate(dot(_vViewNormal, vLight));

	// �ݻ籤 -> ī�޶� ������ �� ���������� �̿� -> ƨ�ܳ����� ���͸� ���� -> ����ȭ
	float4 vReflect = normalize(2.f * dot(vLight, _vViewNormal) * _vViewNormal - vLight);
	float4 vEye = -normalize(_vViewPos);


	CALL.Spec = _Info.Lc.Spec * pow(saturate(dot(vEye, vReflect)), 10);
	CALL.Ambi = _Info.Lc.Ambi;

	return CALL;
}


LightColor Point_Light(float4 _vViewPos, float4 _vViewNormal, LightData _Info)
{
	LightColor CALL = (LightColor)0.f;

	// ����Ʈ�� �ܼ� �����Ǹ� ��� - ������ ���� ����
	// ���� ���ִ� ������ -> ���� ��� ��ǥ�̱� ������ ����
	// �̰�... �ֵ� �ʿ���� �׳� g_WVP�� ����� ��ü��ǥ�� �ʿ��� -> ���� ��ġ���� �������ϴ� �κ��̴ϱ�
	// �ٵ� �������� �������� 1���� ī�޶�� ���̿����̴µ� �����Ұ�.
	matrix TempMat = g_W * g_V;

	float4 vLight = normalize(
		float4(_Info.Pos.x, _Info.Pos.y, _Info.Pos.z, .0f) -
		float4(TempMat._41, TempMat._42, TempMat._43, .0f));

	// Range
	CALL.Diff = _Info.Lc.Diff * saturate(dot(_vViewNormal, vLight));

	// �ݻ籤 -> ī�޶� ������ �� ���������� �̿� -> ƨ�ܳ����� ���͸� ���� -> ����ȭ
	float4 vReflect = normalize(2.f * dot(vLight, _vViewNormal) * _vViewNormal - vLight);
	float4 vEye = -normalize(_vViewPos);

	// float4 vEye = normalize(
	// 	float4(_Info.CamPos.x, _Info.CamPos.y, _Info.CamPos.z, .0f) -
	// 	float4(TempMat._41, TempMat._42, TempMat._43, .0f));


	CALL.Spec = _Info.Lc.Spec * pow(saturate(dot(vEye, vReflect)), 10);
	CALL.Ambi = _Info.Lc.Ambi;

	return CALL;
}