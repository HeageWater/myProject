cbuffer ConstBufferDataMaterial:register(b0)
{
	float4 color;
};

cbuffer ConstBufferDataTransform:register(b1)
{
	matrix mat;
};

cbuffer ConstBufferDataMaterial:register(b2)
{
	float time;
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal :	NORMAL;
	float2 uv :TEXCOORD;
};