#include "Basic.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);

//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//右下奥向きのライト
	float3 light = normalize(float3(1,-1,1));
	//
	float diffuse = saturate(dot(-light, input.normal));
	//光源へのベクトルと法線ベクトルの内積
	//float brightness = dot(-light,input.normal);
	float brightness = diffuse + 0.3f;


	float4 texcolor = float4(tex.Sample(smp, input.uv));

	//輝度をRGBに代入して出力
	return float4(texcolor.rgb * brightness, texcolor.a) * color;
	//return float4(brightness, brightness, brightness, 1);


	//rgbの法線のxyz,aを1で出力
	//return float4(input.normal,1);

	//画像
	//return float4(tex.Sample(smp,input.uv));

	//色付き
	//return float4(tex.Sample(smp,input.uv)) * color;

	//真っ白
	//return float4(1,1,1,1);
}