#pragma once
#include <Windows.h>
#include <thread>
#include <d3dcompiler.h>
#include <D3dx12.h>
#include <string>
#include <DirectXMath.h>
#include <DIrectXTex.h>
#include <wrl.h>
#include <cassert>
#include "DirectXCommon.h"

using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")

//頂点データ構造体
struct VertexPosUv {
	XMFLOAT3 pos;	//xyz座標
	XMFLOAT2 uv;	//uv座標
};

//パイプラインセット
struct PipelineSet {
	//パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
};

//スプラインの共通データ
struct SpriteCommon {
	//テクスチャの最大枚数
	static const int spriteSRVCount = 512;
	//パイプラインセット
	PipelineSet pipelineSet;
	//射影行列
	XMMATRIX matProject{};
	//テクスチャ用でデスクリプタヒープの生成
	ComPtr<ID3D12DescriptorHeap> descHeap;
	//テクスチャソース(テクスチャバッファ)の配列
	ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
	//テクスチャバッファ取得
	ComPtr<ID3D12Resource> GetTextureBuffer(uint32_t index) const { return texBuff[index].Get(); }
};

class Sprite {
public:

	PipelineSet object3dPipelineSet(ID3D12Device* device);

	/// <summary>
	/// スプライト生成
	/// </summary>
	void SpriteCreate(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// スプライン共通グラフィックスコマンドのセット
	/// </summary>
	void SpriteCommonBeginDraw(ID3D12GraphicsCommandList* commandList, const SpriteCommon& spriteCommon);

	/// <summary>
	/// スプライト単体描画
	/// </summary>
	void SpriteDraw(ID3D12GraphicsCommandList* commandlist_, const SpriteCommon& spriteCommon,
					ID3D12Device* device, D3D12_VERTEX_BUFFER_VIEW& rbView);

	/// <summary>
	/// スプライト単体更新
	/// </summary>
	void SpriteUpdate(Sprite& sprite, const SpriteCommon& spriteCommon);

	/// <summary>
	/// スプライト共通テクスチャ読み込み
	/// </summary>
	void SpriteCommonLoadTexture(SpriteCommon& spriteCommon, UINT texnumber, const wchar_t* filename, ID3D12Device* device);

	/// <summary>
	/// スプライト単体頂点バッファの転送
	/// </summary>
	void SpriteTransferVertexBuffer(const Sprite& sprite);

	/// <summary>
	/// スプライト共通データ生成
	/// </summary>
	SpriteCommon spriteCommonCreate(ID3D12Device* device, int window_width, int window_height);

	//------セッター------//
	void SetTexNumber(UINT texNum) { this->texNumber = texNum; }
	//座標設定
	void SetPosition(XMFLOAT2 position) { this->position_ = position; }	
	//回転設定
	void SetRotation(float rotation) { this->rotation_ = rotation; }
	//大きさ設定
	void SetScale(XMFLOAT2 scale) { this->scale_ = scale; }
	//色
	void SetColor(XMFLOAT4 color) { this->color_ = color; }
	//アンカーポイント
	void SetAnchorPoint(XMFLOAT2 anchorPoint) { this->anchorPoint_ = anchorPoint; }
	//左右フリップ
	void SetFlipX(bool flipX) { this->isFlipX_ = flipX; }
	//上下フリップ
	void SetFlipY(bool flipY) { this->isFlipY_ = flipY; }
	//非表示
	void SetInvisible(bool invisible) { this->isInvisible_ = invisible; }
	//テクスチャ左上座標
	void SetTextureLeftTop(XMFLOAT2 textureLeftTop) { this->textureLeftTop_ = textureLeftTop; }
	//テクスチャ切り出しサイズ
	void SetTextureSize(XMFLOAT2 textureSize) { this->textureSize_ = textureSize; }

	//------ゲッター------//
	//座標
	const XMFLOAT2& GetPosition() const { return position_; }
	//回転　
	float GetRotation() const { return rotation_; }
	//色
	XMFLOAT4 GetColor() const { return color_; }
	//アンカーポイント
	XMFLOAT2 GetAnchorPoint() const { return anchorPoint_; }
	//左右フリップ
	bool GetFlipX() const { return isFlipX_; }
	//上下フリップ
	bool GetFlipY() const { return isFlipY_; }
	//非表示
	bool Invisible() const { return isInvisible_; }
	//テクスチャ左上座標
	XMFLOAT2 GetTextureLeftTop() const { return textureLeftTop_; }
	//テクスチャ切り出しサイズ
	XMFLOAT2 GetTextureSize() const { return textureSize_; }

	void Release();

	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

private:

	//テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

	//定数バッファ用データ構造体
	struct ConstBufferData {
		XMFLOAT4 color; //色(RGBA)
		XMMATRIX mat; //3D変換行列
	};

	UINT texNumber = 0;

	DirectXCommon* dXCommon = nullptr;
	ComPtr<ID3D12DescriptorHeap> descHeap = nullptr;
	ComPtr<ID3D12Resource> constBuff = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

	// ワールド行列
	XMMATRIX matWorld;

	//スケーリング倍率
	XMFLOAT2 scale_ = { 1.0f,1.0f };
	//回転角
	float rotation_ = 0.0f;
	//座標
	XMFLOAT2 position_ = { 0.0f,0.0f };
	//色
	XMFLOAT4 color_ = { 1,1,1,1 };
	//アンカーポイント
	XMFLOAT2 anchorPoint_ = { 0.0f,0.0f };

	//テクスチャ左上座標
	XMFLOAT2 textureLeftTop_ = { 0.0f,0.0f };
	//テクスチャ切り出しサイズ
	XMFLOAT2 textureSize_ = { 100.0f,100.0f };

	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;

	//非表示フラグ
	bool isInvisible_ = false;

	float angle = 0.0f;

	DirectXCommon* dxCommon = nullptr;
};


