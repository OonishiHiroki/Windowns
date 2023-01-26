#include "SpriteCommon.h"
#include <DirectXMath.h>
using namespace DirectX;
void SpriteCommon::Initialize(DirectXCommon* dxCommon) {
	//頂点データ
	XMFLOAT3 vertices[] = {
		{-0.5f,-0.5f,0.0f}, //左下
		{-0.5f,+0.5f,0.0f}, //左上
		{+0.5f,-0.5f,0.0f}, //右下
	};
	//頂点データの全体のサイズ = 頂点データ1つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
}