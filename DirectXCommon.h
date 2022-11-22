#pragma once
#include "WinApp.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include <vector>

using namespace Microsoft::WRL;

class DirectXCommon {
public:
	//初期化
	void Initialize(WinApp* winApp);

	//デバイスの初期化
	void InitializeDevice();

	//コマンド関連の初期化
	void InitializeCommand();

	//スワップチェーンの初期化
	void InitializeSwapchain();

	//レンダーターゲットビューの初期化
	void InitializeRenderTargetView();

	//深度バッファの初期化
	void InitializeDepthBuffer();

	//フェンスの初期化
	void InitializeFence();


private:
	//WindowsAPI
	WinApp* winApp = nullptr;
	HRESULT result;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12Fence> fence;

	//DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	//DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	//バックバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

private:
};

