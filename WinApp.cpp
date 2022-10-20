#include "WinApp.h"
#include <Windows.h>


//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)


LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//���b�Z�[�W�ŕ���
	switch (msg) {
		case WM_DESTROY: //�E�B���h�E���j�󂳂ꂽ
			PostQuitMessage(0); //os�ɑ΂��ăA�v���̏I����`����
			return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void WinApp::Initialize() {
	// �E�B���h�E����
	//const int window_width = 1280;
	// �E�B���h�E�c��
	//const int window_height = 720;


	//WNDCLASSEX w = {};
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.lpfnWndProc = (WNDPROC)WindowProc;	//�E�B���h�E�v���V�[�W����ݒ�
	winClass.lpszClassName = L"DX12Sample";			//�E�B���h�E�N���X��
	winClass.hInstance = GetModuleHandle(nullptr);		//�E�B���h�E�n���h��
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//�J�[�\���w��

	//�E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&winClass);
	//�E�B���h�E�T�C�Y{X���W�@Y���W�@�����@�c��}
	RECT wrc = { 0,0,window_width,window_height };
	//�֐����g���ăE�B���h�E�̃T�C�Y�������ŕ␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(winClass.lpszClassName,//�N���X���w��
						L"LE2B_07_�I�I�j�V_�q���L_GE3",					//�^�C�g���o�[�̕���
						WS_OVERLAPPEDWINDOW,			//�^�C�g���o�[�Ƌ��E��������E�B���h�E
						CW_USEDEFAULT,					//�\��x���W��OS�ɂ��C��
						CW_USEDEFAULT,					//�\��y���W��OS�ɂ��C��
						wrc.right - wrc.left,			//�E�B���h�E��
						wrc.bottom - wrc.top,			//�E�B���h�E��
						nullptr,						//�e�E�B���h�E�n���h��
						nullptr,						//���j���[�n���h��
						winClass.hInstance,					//�Ăяo���A�v���P�[�V�����n���h��
						nullptr);						//�ǉ��p�����[�^�[(�I�v�V����)

					//�E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

void WinApp::Update() {

}

void WinApp::Finalize() {
	//�E�B���h�E�N���X��o�^����
	UnregisterClass(winClass.lpszClassName, winClass.hInstance);
}

bool WinApp::ProcessMessage() {

	MSG msg{};

	//���b�Z�[�W������H
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (msg.message == WM_QUIT) {
		return true;
	}
	return false;
}
