// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "MatrixContainer.fx"
#include "PixelContainer.fx"
#include "LightContainer.fx"
#include "TextureContainer.fx"

struct DEFMESH_VT_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vNormal : NORMAL;
    float4 vTangent : TAN; // U�� ��ġ�ϴ� X��
    float4 vBNormal : BTAN; // V�� ��ġ�ϴ� ź��Ʈ
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
struct DEFMESH_VT_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
    float4 vColor : COLOR;
    float4 vViewPos : POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TAN; // U�� ��ġ�ϴ� X��
    float4 vBNormal : BTAN; // V�� ��ġ�ϴ� ź��Ʈ
};


DEFMESH_VT_OUT DefMesh_VT(DEFMESH_VT_IN _in)
{
    DEFMESH_VT_OUT outData = (DEFMESH_VT_OUT) 0.f;

    outData.vPos = mul(_in.vPos, g_WVP);
    outData.vUv = _in.vUv;
    outData.vColor.rgba = _in.vColor.rgba;
    outData.vViewPos = mul(_in.vPos, g_WV);
    outData.vNormal = normalize(mul(_in.vNormal, g_WV));
    outData.vTangent = normalize(mul(_in.vTangent, g_WV));
    outData.vBNormal = normalize(mul(_in.vBNormal, g_WV));

    if (0 == CheckLight)
    {
        return outData;
    }

    return outData;
}

DEFMESH_PX_OUT DefMesh_PX(DEFMESH_VT_OUT _in)
{
    DEFMESH_PX_OUT outData = (DEFMESH_PX_OUT) 0.0f;
    outData.vDiffuse = _in.vColor;
    float4 CalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    for (int i = 0; i < TexCnt; ++i)
    {
        if (-1 != Texes[i].TInx)
        {
            if (Texes[i].Type == TEXCOLOR)
            {
                CalColor *= Get_CalColor(Texes[i].TInx, Texes[i].TSmp, _in.vUv) * _in.vColor;
            }
            else if (Texes[i].Type == TEXBUMP)
            {
                _in.vNormal = Set_Bump(Texes[i].TInx, Texes[i].TSmp, _in.vUv, _in.vTangent, _in.vBNormal, _in.vNormal);
            }
        }
    }

    // ������ ������ �ƿ� ������� �ʴ� ���� �ƴϴ�.
    outData.vDiffuse.rgb = CalColor;
    outData.vDiffuse.a = _in.vColor.a;
    outData.vNoraml = _in.vNormal;
    outData.vNoraml.a = 1.0f;
    outData.vPosition = _in.vViewPos;
    outData.vDepth.xyzw = outData.vPosition.z;

	// outData.vColor = _in.vColor;
    return outData;
}