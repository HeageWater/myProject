//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

//マテリアル
cbuffer ConstBufferDataMaterial : register(b0)
{
	float4 color;
};

struct VSOutput
{
	//システム用頂点座標
	float4 svpos : SV_POSITION;

	//uv値
	float2 uv :TEXCOORD;
};