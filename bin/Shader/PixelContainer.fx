// ���۵�� �̷��� �Ѳ����� �޴� �� Ư¡�Ӥ���
struct PS_DEFFERDOUTPUT
{
    float4 vDiffuse : SV_Target;
    float4 vPosition : SV_Target1;
    float4 vNoraml : SV_Target2;
    float4 vDepth : SV_Target3;
    // float4 vDepth       : SV_Target4; // ���� �׸���.
};

struct PS_FORWARDOUTPUT
{
    float4 vDiffuse : SV_Target; // ����.
};