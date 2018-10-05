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
    int Dump;
};

cbuffer LightBuffer : register(b10)
{
	LightData LightList[10];
	int       LightCount;
	int       Temp1;
	int       Temp2;
	int       Temp3;
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




float3 Direct_Light(float4 _vViewPos, float4 _vViewNormal, LightData _Info)
{
    float3 LC = float3(1.0f, 1.0f, .0f);

    // �� ��ǻ��
    float NDotL = dot(_Info.Dir, _vViewNormal);
    float3 CalCol = LC * saturate(NDotL);

    // �� ����ŧ��
    float3 ToEye = _Info.CamPos.xyz - _vViewPos.xyz;
    ToEye = normalize(ToEye);
    float3 HalfWay = normalize(ToEye + _Info.Dir.xyz);
    float NDotH = saturate(dot(HalfWay, _vViewNormal.xyz));
    // ����ŧ�� - ��� ��ġ�� ���� ���� �ݻ簡 �۴�
    // CalCol += LC * pow(NDotH, _Info.Lc.Spec.xyz) * 0.00001f;

    // ȯ�汤
    CalCol += _Info.Lc.Ambi.xyz;

    return CalCol * _Info.Lc.Diff.xyz;

}



float3 Point_Light(float4 _vViewPos, float4 _vViewNormal, LightData _Info)
{
    float3 LC = float3(.0f, .0f, 1.0f);

    float3 ToLight = _Info.Pos.xyz - _vViewPos.xyz;
    float3 ToEye = _Info.CamPos.xyz - _vViewPos.xyz;
    float DistToLight = length(ToLight);


    // �� ��ǻ��
    ToLight /= DistToLight;
    float NDotL = dot(ToLight, _vViewNormal.xyz);
    float3 CalCol = LC * NDotL;

    // �� ����ŧ��
    ToEye = normalize(ToEye);
    float3 HalfWay = normalize(ToEye + ToLight);
    float NDotH = saturate(dot(HalfWay, _vViewNormal.xyz));
    // ����ŧ�� - ��� ��ġ�� ���� ���� �ݻ簡 �۴� - �ƿ� ���� == �ǻ籤 ����
    // CalCol += LC * pow(NDotH, _Info.Lc.Spec.xyz) * 0.00001f;

    // ȯ�汤
    CalCol += _Info.Lc.Ambi.xyz;

    // ����
    //float DistToLightNorm = 1.0f - saturate(DistToLight * 50.0f);
    //float Attn = DistToLightNorm * DistToLightNorm;

    return CalCol * _Info.Lc.Diff.xyz /** Attn*/;
}