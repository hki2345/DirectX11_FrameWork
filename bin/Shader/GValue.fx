
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
    int IsDefferdAndForward; // 디퍼느냐 포워드냐?
	int LightVtxToPix; // 버텍스냐 픽셀이냐? // 0 이면 버텍스 1이면 픽셀
    int TexCount; // 버텍스냐 픽셀이냐? // 8
    int IsAni; // 버텍스냐 픽셀이냐? // 8
    int Temp1; // 버텍스냐 픽셀이냐? // 8
    int Temp2; // 버텍스냐 픽셀이냐? // 8
    int Temp3; // 버텍스냐 픽셀이냐? // 8
    TexData ArrTex[8]; // 20 216
}
