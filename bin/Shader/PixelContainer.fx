// ���۵�� �̷��� �Ѳ����� �޴� �� Ư¡�Ӥ���
struct DEFMESH_PX_OUT
{
    float4 vDiffuse : SV_Target;
    float4 vPosition : SV_Target1;
    float4 vNoraml : SV_Target2;
    float4 vDepth : SV_Target3;
    // float4 vDepth       : SV_Target4; // ���� �׸���.
};

struct FORMESH_PX_OUT
{
    float4 vDiffuse : SV_Target; // ����.
};