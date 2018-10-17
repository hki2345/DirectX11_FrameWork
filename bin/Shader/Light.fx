// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
class LIGHTCOLOR
{
	float4 Diff; // 확산광
	float4 Spec; // 반사광
	float4 Ambi; // 환경광
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

