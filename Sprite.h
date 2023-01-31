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
	//�e�N�X�`���o�b�t�@�擾
	ComPtr<ID3D12Resource> GetTextureBuffer(uint32_t index) const { return texBuff[index].Get(); }
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

	//------�Z�b�^�[------//
	void SetTexNumber(UINT texNum) { this->texNumber = texNum; }
	//���W�ݒ�
	void SetPosition(XMFLOAT2 position) { this->position_ = position; }	
	//��]�ݒ�
	void SetRotation(float rotation) { this->rotation_ = rotation; }
	//�傫���ݒ�
	void SetScale(XMFLOAT2 scale) { this->scale_ = scale; }
	//�F
	void SetColor(XMFLOAT4 color) { this->color_ = color; }
	//�A���J�[�|�C���g
	void SetAnchorPoint(XMFLOAT2 anchorPoint) { this->anchorPoint_ = anchorPoint; }
	//���E�t���b�v
	void SetFlipX(bool flipX) { this->isFlipX_ = flipX; }
	//�㉺�t���b�v
	void SetFlipY(bool flipY) { this->isFlipY_ = flipY; }
	//��\��
	void SetInvisible(bool invisible) { this->isInvisible_ = invisible; }
	//�e�N�X�`��������W
	void SetTextureLeftTop(XMFLOAT2 textureLeftTop) { this->textureLeftTop_ = textureLeftTop; }
	//�e�N�X�`���؂�o���T�C�Y
	void SetTextureSize(XMFLOAT2 textureSize) { this->textureSize_ = textureSize; }

	//------�Q�b�^�[------//
	//���W
	const XMFLOAT2& GetPosition() const { return position_; }
	//��]�@
	float GetRotation() const { return rotation_; }
	//�F
	XMFLOAT4 GetColor() const { return color_; }
	//�A���J�[�|�C���g
	XMFLOAT2 GetAnchorPoint() const { return anchorPoint_; }
	//���E�t���b�v
	bool GetFlipX() const { return isFlipX_; }
	//�㉺�t���b�v
	bool GetFlipY() const { return isFlipY_; }
	//��\��
	bool Invisible() const { return isInvisible_; }
	//�e�N�X�`��������W
	XMFLOAT2 GetTextureLeftTop() const { return textureLeftTop_; }
	//�e�N�X�`���؂�o���T�C�Y
	XMFLOAT2 GetTextureSize() const { return textureSize_; }

	void Release();

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

private:

	//�e�N�X�`���T�C�Y���C���[�W�ɍ��킹��
	void AdjustTextureSize();

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
	XMFLOAT2 scale_ = { 1.0f,1.0f };
	//��]�p
	float rotation_ = 0.0f;
	//���W
	XMFLOAT2 position_ = { 0.0f,0.0f };
	//�F
	XMFLOAT4 color_ = { 1,1,1,1 };
	//�A���J�[�|�C���g
	XMFLOAT2 anchorPoint_ = { 0.0f,0.0f };

	//�e�N�X�`��������W
	XMFLOAT2 textureLeftTop_ = { 0.0f,0.0f };
	//�e�N�X�`���؂�o���T�C�Y
	XMFLOAT2 textureSize_ = { 100.0f,100.0f };

	//���E�t���b�v
	bool isFlipX_ = false;
	//�㉺�t���b�v
	bool isFlipY_ = false;

	//��\���t���O
	bool isInvisible_ = false;

	float angle = 0.0f;

	DirectXCommon* dxCommon = nullptr;
};


