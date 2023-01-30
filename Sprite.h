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

//���_�f�[�^�\����
struct VertexPosUv {
	XMFLOAT3 pos;	//xyz���W
	XMFLOAT3 normal;//�@���x�N�g��
	XMFLOAT2 uv;	//uv���W
};

//�p�C�v���C���Z�b�g
struct PipelineSet {
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelineState;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;
};

//�X�v���C���̋��ʃf�[�^
struct SpriteCommon {
	//�e�N�X�`���̍ő喇��
	static const int spriteSRVCount = 512;
	//�p�C�v���C���Z�b�g
	PipelineSet pipelineSet;
	//�ˉe�s��
	XMMATRIX matProject{};
	//�e�N�X�`���p�Ńf�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeap;
	//�e�N�X�`���\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
};

class Sprite {
public:

	PipelineSet object3dPipelineSet(ID3D12Device* device);

	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	void SpriteCreate(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// �X�v���C�����ʃO���t�B�b�N�X�R�}���h�̃Z�b�g
	/// </summary>
	void SpriteCommonBeginDraw(ID3D12GraphicsCommandList* commandList, const SpriteCommon& spriteCommon);

	/// <summary>
	/// �X�v���C�g�P�̕`��
	/// </summary>
	void SpriteDraw(ID3D12GraphicsCommandList* commandlist_, const SpriteCommon& spriteCommon,
					ID3D12Device* device, D3D12_VERTEX_BUFFER_VIEW& rbView);

	/// <summary>
	/// �X�v���C�g�P�̍X�V
	/// </summary>
	void SpriteUpdate(Sprite& sprite, const SpriteCommon& spriteCommon);

	/// <summary>
	/// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	/// </summary>
	void SpriteCommonLoadTexture(SpriteCommon& spriteCommon, UINT texnumber, const wchar_t* filename, ID3D12Device* device);

	/// <summary>
	/// �X�v���C�g�P�̒��_�o�b�t�@�̓]��
	/// </summary>
	void SpriteTransferVertexBuffer(const Sprite& sprite);

	/// <summary>
	/// �X�v���C�g���ʃf�[�^����
	/// </summary>
	SpriteCommon spriteCommonCreate(ID3D12Device* device, int window_width, int window_height);

	void SetTexNumber(UINT texNum) { this->texNumber = texNum; }

	//���W�ݒ�
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	//�傫���ݒ�
	void SetScale(XMFLOAT2 scale) { this->scale = scale; }

	void Release();

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

private:
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		XMFLOAT4 color; //�F(RGBA)
		XMMATRIX mat; //3D�ϊ��s��
	};

	UINT texNumber = 0;

	DirectXCommon* dXCommon = nullptr;
	ComPtr<ID3D12DescriptorHeap> descHeap = nullptr;
	ComPtr<ID3D12Resource> constBuff = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

	// ���[���h�s��
	XMMATRIX matWorld;

	//�X�P�[�����O�{��
	XMFLOAT2 scale = { 1.0f,1.0f };

	//��]�p
	float rotation = 0.0f;

	//���W
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };

	float angle = 0.0f;

	DirectXCommon* dxCommon = nullptr;
};


