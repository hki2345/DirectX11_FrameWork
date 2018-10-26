
cbuffer TransformBuffer : register(b10)
{
	matrix g_W;
	matrix g_V;
	matrix g_P;
    matrix g_WV;
	matrix g_WVP;
}

class TexData
{
    int Type;
    int Tex_Idx;
    int Tex_Smp;
    int Temp;
};

cbuffer RenderOption : register(b11)
{
	int IsLight;
    int IsDefferdAndForward; // ���۴��� �������?
	int LightVtxToPix; // ���ؽ��� �ȼ��̳�? // 0 �̸� ���ؽ� 1�̸� �ȼ�
    int TexCount; // ���ؽ��� �ȼ��̳�? // 8
    int IsAni; // ���ؽ��� �ȼ��̳�? // 8
    int Temp1; // ���ؽ��� �ȼ��̳�? // 8
    int Temp2; // ���ؽ��� �ȼ��̳�? // 8
    int Temp3; // ���ؽ��� �ȼ��̳�? // 8
    TexData ArrTex[8]; // 20 216
}
