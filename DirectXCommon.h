#pragma once
#include "WinApp.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include <vector>

using namespace Microsoft::WRL;

class DirectXCommon {
public:
	//������
	void Initialize(WinApp* winApp);

	//�f�o�C�X�̏�����
	void InitializeDevice();

	//�R�}���h�֘A�̏�����
	void InitializeCommand();

	//�X���b�v�`�F�[���̏�����
	void InitializeSwapchain();

	//�����_�[�^�[�Q�b�g�r���[�̏�����
	void InitializeRenderTargetView();

	//�[�x�o�b�t�@�̏�����
	void InitializeDepthBuffer();

	//�t�F���X�̏�����
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

	//DirectX12�f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	//DXGI�t�@�N�g��
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	//�o�b�N�o�b�t�@
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

private:
};

