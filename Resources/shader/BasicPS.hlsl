#include "Basic.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);

//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���}�b�s���O
	float4 texcolor = float4(tex.Sample(smp, input.uv));

	//�E���������̃��C�g
	float3 light = normalize(float3(1,-1,1));

	//
	float diffuse = saturate(dot(-light, input.normal));

	//�����ւ̃x�N�g���Ɩ@���x�N�g���̓���
	float brightness = diffuse + 0.3f;

	//�P�x��RGB�ɑ�����ďo��
	//return float4(texcolor.rgb * brightness, texcolor.a) * color; 
	return float4(texcolor.rgb * brightness, texcolor.z);
}