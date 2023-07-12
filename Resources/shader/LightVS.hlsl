#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;
	output.svpos = mul(mat, pos);
	output.pos = mul(pos, rotMat);
	output.normal = mul(normal, rotMat);
	output.uv = uv;
	return output;
}
