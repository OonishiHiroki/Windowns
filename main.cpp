//#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial {
	XMFLOAT4 color;//�F(RGBA)
	XMMATRIX mat; //3D�ϊ��s��
};

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

#pragma region WindowCreate

	MSG msg{};	//���b�Z�[�W

	//-------DirectX�����������@��������-------//

	//�|�C���^
	Input* input = nullptr;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;

	//WindowsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

	//���͂̏�����
	input = new Input();
	input->Initialize(winApp);

	//DirectX�̏�����
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);


	//-------DirectX�����������@�����܂�-------//


	//-------�`�揉���������@�����܂�-------//

	Sprite* sprite = new Sprite();
	Sprite title;
	SpriteCommon spriteCommon_;
	spriteCommon_ = sprite->spriteCommonCreate(dxCommon->GetDevice(), 1280, 720);
	sprite->SpriteCommonLoadTexture(spriteCommon_, 0, L"Resource/micel.jpg", dxCommon->GetDevice());

	title.SetColor(XMFLOAT4(1, 0, 1, 1));
	title.SpriteCreate(dxCommon->GetDevice(), 1280, 720);
	title.SetTexNumber(0);
	title.SetPosition(XMFLOAT2(640, 360));
	title.SetAnchorPoint(XMFLOAT2(0.5, 0.5));
	title.SetScale(XMFLOAT2(1280 * 0.5, 720 * 0.5));
	title.SpriteTransferVertexBuffer(title);
	title.SpriteUpdate(title, spriteCommon_);

	//�Q�[�����[�v
	while (true) {
		//Window�̃��b�Z�[�W����
		if (winApp->ProcessMessage()) {
			//�Q�[�����[�v�𔲂���
			break;
		}
		//-------DirectX���t���[�������@��������-------//

		input->Update();

		//������0�L�[�������ꂽ��
		if (input->PushKey(DIK_0)) {
			OutputDebugStringA("Hit 0\n");
		}

		//if (input->PushKey(DIK_SPACE)) //�X�y�[�X�L�[�������ꂽ��
		//{
		//	clearColor[0] = { 0.5f };
		//	clearColor[1] = { 0.5f };
		//	clearColor[2] = { 0.5f };
		//	clearColor[3] = { 0.5f };
		//}

		//if (!input->PushKey(DIK_SPACE)) //�X�y�[�X�L�[�������ꂽ��
		//{
		//	clearColor[0] = { 0.1f };
		//	clearColor[1] = { 0.25f };
		//	clearColor[2] = { 0.5f };
		//	clearColor[3] = { 0.0f };
		//}

		//�����ꂩ�̃L�[�������Ă�����

		////���W���ړ����鏈��(Z���W)
		//if (input->PushKey(DIK_UP)) {
		//	position.z += 1.0f;
		//}
		//else if (input->PushKey(DIK_DOWN)) {
		//	position.z -= 1.0f;
		//}
		//if (input->PushKey(DIK_RIGHT)) {
		//	position.x += 1.0f;
		//}
		//else if (input->PushKey(DIK_LEFT)) {
		//	position.x -= 1.0f;
		//}



		//if (input->PushKey(DIK_D) || input->PushKey(DIK_A)) {
		//	if (input->PushKey(DIK_D)) { angle += XMConvertToRadians(1.0f); }
		//	else if (input->PushKey(DIK_A)) { angle -= XMConvertToRadians(1.0f); }

		//	//angle���W�A������������ɉ�]�B���a��-100
		//	eye.x = -200 * (sinf(angle));
		//	eye.z = -200 * (cosf(angle));
		//	/*rotation.x = -200 * (sinf(angle));
		//	rotation.y = -200 * (cosf(angle));*/
		//	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		//}

		//constMapTransform0->mat = matWorld0 * matView * matProjection;

		//�`��O����
		dxCommon->PreDraw();

		//-------4.�`��R�}���h��������-------//
		sprite->SpriteCommonBeginDraw(dxCommon->GetCommandList(), spriteCommon_);
		title.SpriteDraw(dxCommon->GetCommandList(), spriteCommon_, dxCommon->GetDevice(), title.vbView);
		////-------4.�`��R�}���h�����܂�-------//

		//�`��㏈��
		dxCommon->PostDraw();

		//-------DirectX���t���[�������@�����܂�-------//
	}

	delete sprite;

	//WindowAPI�̏I������
	winApp->Finalize();

	//�e����
	delete input;
	delete winApp;
	delete dxCommon;

	////�E�B���h�E�N���X��o�^����
	//UnregisterClass(w.lpszClassName, w.hInstance);


	//delete[] scratchImg;

	return 0;
}