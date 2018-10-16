// ������۴� b
// �ؽ�ó�� t
// ���÷��� s
#include "MatrixContainer.fx"
#include "PixelContainer.fx"
#include "LightDefferd.fx"
#include "TextureContainer.fx"

struct SCRMERGE_VT_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
};

// ���ؽ� ���̴����� ������ ����ü�� �����.
// SV_�� �ٰԵǸ� cosnt�� �ٿ��� �׷��� ������ �̵����Ϳ� ���ؼ� ���̻���
// ������ �Ұ����ϰ� �Ѵ�.
struct SCRMERGE_VT_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
};


// .5f �� �����ϱ� 2��� ������ �Ѵ�.
static matrix DirMat =
{
    { 2.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 2.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 2.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};

SCRMERGE_VT_OUT ScrMerge_VT(SCRMERGE_VT_IN _Input)
{
    SCRMERGE_VT_OUT OUTDATA = (SCRMERGE_VT_OUT) 0.0F;
    OUTDATA.vPos = mul(_Input.vPos, DirMat);
    OUTDATA.vUv = _Input.vUv;
    return OUTDATA;
}


MERGE_PX_OUT ScrMerge_PX(SCRMERGE_VT_OUT _Input)
{
    MERGE_PX_OUT OUTDATA = (MERGE_PX_OUT) 0.0F;
    OUTDATA.vMergeColor = g_Tex_0.Sample(g_Sam_0, _Input.vUv);
    return OUTDATA;
}