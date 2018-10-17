// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
class LIGHTCOLOR
{
	float4 Diff; // Ȯ�걤
	float4 Spec; // �ݻ籤
	float4 Ambi; // ȯ�汤
};

class LIGHTDATA
{
	LIGHTCOLOR Lc;
	float4 LightPos;
	float4 LightDir;
	float Range;
	float Angle;
	int Type;
    int Dump;
};

LIGHTCOLOR CalDirLight(float4 _vViewPos, float4 _vViewNormal, LIGHTDATA _LightInfo)
{
	LIGHTCOLOR CALCOLOR;
	float4 vLight = -_LightInfo.LightDir;

	CALCOLOR.Diff = _LightInfo.Lc.Diff * saturate(dot(_vViewNormal, normalize(vLight)));
	vLight = normalize(vLight);

	float3 vReflect = normalize(2.f * dot(vLight, _vViewNormal) * _vViewNormal - vLight);
	float3 vEye = normalize(-_vViewPos);

	CALCOLOR.Spec = _LightInfo.Lc.Spec * pow(saturate(dot(vEye, vReflect)), 1);
	CALCOLOR.Ambi = _LightInfo.Lc.Ambi;

	return CALCOLOR;
}

