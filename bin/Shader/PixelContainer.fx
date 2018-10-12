// ���۵�� �̷��� �Ѳ����� �޴� �� Ư¡�Ӥ���
struct DEFMESH_PX_OUT
{
    float4 vDiffuse     : SV_Target;
    float4 vPosition    : SV_Target1;
    float4 vNoraml      : SV_Target2;
    float4 vDepth       : SV_Target3;
    // float4 vDepth       : SV_Target4; // ���� �׸���.
};

struct FORMESH_PX_OUT
{
    float4 vDiffuse : SV_Target; // ����.
};

struct MERGE_PX_OUT
{
    float4 vMergeColor : SV_Target; // ����.
};




struct DEFFERDLIGHT_VT_IN
{
    float4 vPos : POSITION;
    float2 vUv : TEXCOORD;
};

struct DEFFERDLIGHT_VT_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUv : TEXCOORD;
};

struct DEFFERDLIGHT_PX_OUT
{
    float4 vDiffuse : SV_Target;
    float4 vSpeculer : SV_Target1;
};