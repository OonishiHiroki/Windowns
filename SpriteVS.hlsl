#include "Sprite.hlsli"
VSOutput main( float4 pos : POSITION, float2 uv : TEXCOORD )
{
	VSOutput output;
	output.svpos = mul(mat, pos);
	output.uv = uv;
	return output;
}

////���_�f�[�^�\����
//struct VertexPosUv {
//	XMFLOAT3 pos;	//xyz���W
//	XMFLOAT2 uv;	//uv���W
//};