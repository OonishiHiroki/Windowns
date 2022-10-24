#include "Sprite.hlsli"
VSOutput main( float4 pos : POSITION, float2 uv : TEXCOORD )
{
	VSOutput output;
	output.svpos = mul(mat, pos);
	output.uv = uv;
	return output;
}

////頂点データ構造体
//struct VertexPosUv {
//	XMFLOAT3 pos;	//xyz座標
//	XMFLOAT2 uv;	//uv座標
//};