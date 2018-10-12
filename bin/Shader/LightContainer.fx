class LightColor
{
    float4 Color; // ���� ��
	float4 Diff;    // Ȯ�걤
	float4 Spec;    // �ݻ籤
	float4 Ambi;    // ȯ�汤
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



LightColor Direct_Light(float4 _vViewPos, float4 _vViewNormal, LightData _Info)
{
    LightColor LD = (LightColor)0.0f;

    // �� ��ǻ��
    float3 LC = _Info.Lc.Color.rgb;
    float NDotL = dot(_Info.Dir, _vViewNormal);
    LD.Diff.rgb = LC * saturate(NDotL);
    LD.Diff.a = 1.0f;
   

    // �� ����ŧ��
    float3 ToEye = _Info.CamPos.xyz - _vViewPos.xyz;
    ToEye = normalize(ToEye);
    float3 HalfWay = normalize(ToEye + _Info.Dir.xyz);
    float NDotH = saturate(dot(HalfWay, _vViewNormal.xyz));
    // ����ŧ�� - ��� ��ġ�� ���� ���� �ݻ簡 �۴�
    LD.Spec.rgb += LC * pow(NDotH, _Info.Lc.Spec.xyz) * 0.00001f;
    LD.Spec.a = 1.0f;

    // ȯ�汤
    //CalCol += _Info.Lc.Ambi.xyz;

    return LD;

}



float3 Point_Light(float4 _vViewPos, float4 _vViewNormal, LightData _Info)
{

    // �� ī�޶� ������ �����ϴ� �� �ǹ��̴�. -> ���� ī�޶� �����̵��� �Ǿ��ִ�.
    // �ش� �������� �ٿ��� ���ؽ� �޽����� ���� �Ŵϱ�...
    // �޽��� �׸� ���� ��ġ���� �� �ְ� �װ� g_WV�� �������鼭 ��ġ�Ӽ����� ������ ��Ȳ
    // ���� ķ������ ��ġ�Ӽ��� �������� ���־�� ������ �ȴ�.
    float3 LC = _Info.Lc.Color.rgb;
    float3 ToLight = _Info.Pos.xyz - _vViewPos.xyz - _Info.CamPos.xyz;
    float3 ToEye = _Info.CamPos.xyz - _vViewPos.xyz;
    float DistToLight = length(ToLight);


    // �� ��ǻ��
    ToLight /= DistToLight;
    float NDotL = dot(ToLight, _vViewNormal.xyz);
    float3 CalCol = LC * NDotL;

    // �� ����ŧ��
    // ToEye = normalize(ToEye);
    // float3 HalfWay = normalize(ToEye + ToLight);
    // float NDotH = saturate(dot(HalfWay, _vViewNormal.xyz));
    // // ����ŧ�� - ��� ��ġ�� ���� ���� �ݻ簡 �۴� - �ƿ� ���� == �ǻ籤 ����
    // CalCol += LC * pow(NDotH, _Info.Lc.Spec.xyz) * 0.01f;

    // ȯ�汤
    CalCol += _Info.Lc.Ambi.xyz;

    // ����
    float DistToLightNorm = 1.0f - saturate(DistToLight * 1 / _Info.Range);
    float Attn = DistToLightNorm * DistToLightNorm;

    return CalCol * _Info.Lc.Diff.xyz * Attn;
}