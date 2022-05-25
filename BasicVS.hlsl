#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;
	output.svpos = pos;
	output.uv = uv;
	return output;
}

//float4 main(float4 pos : POSITION) : SV_POSITION
//{
//	return pos + float4(0.5f,0.3f,0,0);
//}