// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "LightContainer.fx"

// Texture2D g_Tex_0 : register(t0);
// SamplerState g_Sam_0 : register(s0);

// ���ؽ� ���ۿ� ���� ������ �����ϴ� 
// POSITION�� �ø�ƽ�̶�� �ϴµ�.
// ���ؽ� ���̴����� �̿��� ��.

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
LIGHT_VT_OUT VS_MESH3D(LIGHT_VT_IN _in)
{
	LIGHT_VT_OUT outData = (LIGHT_VT_OUT)0.f;

	// �ȼ����� ���ؽ� ����� �ƴ� �ȼ� �ϳ��ϳ��� ����̵�����
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





	// �ȼ� �ϳ��ϳ� ����� �� �� ����� �Ͽ� �����Ѵ�.
	outData.vColor.rgb = _in.vColor.rgb * LC.Diff.rgb + LC.Spec.rgb + LC.Ambi;
	outData.vColor.a = _in.vColor.a;

	return outData;
}