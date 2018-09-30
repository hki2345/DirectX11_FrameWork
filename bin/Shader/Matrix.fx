// 거진 쉐이더파일 모든 곳에서 필요한 것
cbuffer TransformBuffer : register(b1)
{
	matrix g_W;
	matrix g_V;
	matrix g_P;
	matrix g_WVP;
}