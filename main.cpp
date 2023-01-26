//#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "SpriteCommon.h"

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

#pragma region WindowCreate //��ՃV�X�e���̏�����

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

	SpriteCommon* spriteCommon = nullptr;
	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	Sprite* sprite = new Sprite();
	sprite->Initialize(dxCommon);

#pragma endregion WindowCreate //��ՃV�X�e���̏I��

	//-------�`�揉���������@�����܂�-------//

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

		//�����ꂩ�̃L�[�������Ă�����

		//���W���ړ����鏈��(Z���W)
		/*if (input->PushKey(DIK_UP)) {
			position.z += 1.0f;
		}
		else if (input->PushKey(DIK_DOWN)) {
			position.z -= 1.0f;
		}
		if (input->PushKey(DIK_RIGHT)) {
			position.x += 1.0f;
		}
		else if (input->PushKey(DIK_LEFT)) {
			position.x -= 1.0f;
		}*/

		//�`��O����
		dxCommon->PreDraw();

		//-------4.�`��R�}���h��������-------//

		//�`��㏈��
		dxCommon->PostDraw();

		//-------DirectX���t���[�������@�����܂�-------//
	}

#pragma region �ŏ��̃V�[��
	

	//WindowAPI�̏I������
	winApp->Finalize();

	//�e����
	delete input;
	delete winApp;
	delete dxCommon;
	delete spriteCommon;
	delete sprite;

	////�E�B���h�E�N���X��o�^����
	//UnregisterClass(w.lpszClassName, w.hInstance);


	//delete[] scratchImg;

	return 0;
}