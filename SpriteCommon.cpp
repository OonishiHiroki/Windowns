#include "SpriteCommon.h"
#include <DirectXMath.h>
using namespace DirectX;
void SpriteCommon::Initialize(DirectXCommon* dxCommon) {
	//���_�f�[�^
	XMFLOAT3 vertices[] = {
		{-0.5f,-0.5f,0.0f}, //����
		{-0.5f,+0.5f,0.0f}, //����
		{+0.5f,-0.5f,0.0f}, //�E��
	};
	//���_�f�[�^�̑S�̂̃T�C�Y = ���_�f�[�^1���̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
}