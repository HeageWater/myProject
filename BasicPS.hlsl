#include "Basic.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);

//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//画像
	return float4(tex.Sample(smp,input.uv));
	
	//色付き
	//return float4(tex.Sample(smp,input.uv)) * color;

	//真っ白
	//return float4(1,1,1,1);
}