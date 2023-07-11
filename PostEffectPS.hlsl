#include "PostEffect.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);

//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);

	//輝度をRGBに代入して出力
	return float4(texcolor.rgb * 2.0f, 1);
}