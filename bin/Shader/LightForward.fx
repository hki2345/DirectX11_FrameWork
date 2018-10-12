#include "LightContainer.fx"

cbuffer LightBuffer : register(b10)
{
    LightData LightList[10];
    int LightCount;
    int Temp1;
    int Temp2;
    int Temp3;
}