#include "Basic.hlsli"

Texture2D<float4> tex: register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp,input.uv));
	float3 light = normalize(input.pos - lightPos);
	float diffuse = saturate(dot(light, input.normal));
	float brightness = diffuse + 0.3;
	return float4(texcolor.rgb * brightness, texcolor.z) * color;
}