// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "MatrixContainer.fx"
#include "PixelContainer.fx"
#include "LightDefferd.fx"
#include "TextureContainer.fx"

struct SCRMERGE_VT_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
};

// 버텍스 쉐이더에서 리턴할 구조체를 만든다.
// SV_가 붙게되면 cosnt를 붙여서 그래픽 적으로 이데이터에 대해서 더이상의
// 변형을 불가능하게 한다.
struct SCRMERGE_VT_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
};


// .5f 로 들어오니까 2배로 만들어야 한다.
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