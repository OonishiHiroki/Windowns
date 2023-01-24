#pragma once
#include "WinApp.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include <vector>
#include <chrono>

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

	//�`��O����
	void PreDraw();

	//�`��㏈��
	void PostDraw();

	//�f�o�C�X�̎擾
	ID3D12Device* GetDevice() const { return device.Get(); }

	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get();}

private:
	//WindowsAPI
	WinApp* winApp = nullptr;
	HRESULT result;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Fence> fence;

	UINT64 fenceVal = 0;

	UINT rtvHD = 0;

	//DirectX12�f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	//DXGI�t�@�N�g��
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	//�o�b�N�o�b�t�@
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	//�����o�֐�
	//FPS�Œ菉����
	void InitializeFixFPS();

	//FPS�Œ�X�V
	void UpdateFixFPS();

	//�L�^����
	std::chrono::steady_clock::time_point reference_;

};
