//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

//�}�e���A��
cbuffer ConstBufferDataMaterial : register(b0)
{
	float4 color;
};

struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;

	//�@���x�N�g��
	float3 normal :NORMAL;

	//uv�l
	float2 uv :TEXCOORD;
};

//3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b1)
{
	matrix mat;
}