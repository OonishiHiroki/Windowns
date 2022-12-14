//#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include<d3dcompiler.h>
#include<string>
#include<DirectXMath.h>
#include<DIrectXTex.h>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")

//�萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferDataMaterial {
	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }; //�F(RGBA)
};

//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform {
	XMMATRIX mat; //3D�ϊ��s��
};

//���_�f�[�^�\����
struct Vertex {
	XMFLOAT3 pos;	//xyz���W
	XMFLOAT3 normal;//�@���x�N�g��
	XMFLOAT2 uv;	//uv���W
};

//�E�B���h�E�v���V�[�W��
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//���b�Z�[�W�ɉ����ăQ�[�����L�̏������s��
	switch (msg) {
		//�E�B���h�E���j�����ꂽ
		case WM_DESTROY:
			//OS�ɑ΂��āA�A�v���̏I����`����
			PostQuitMessage(0);
			return 0;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

#pragma region WindowCreate
	////�E�B���h�E�T�C�Y
	//const int window_width = 1280; //����
	//const int window_height = 720; //�c��

	////�E�B���h�E�N���X�̐ݒ�
	//WNDCLASSEX w{};
	//w.cbSize = sizeof(WNDCLASSEX);
	//w.lpfnWndProc = (WNDPROC)WindowProc;
	//w.lpszClassName = L"DirectX";
	//w.hInstance = GetModuleHandle(nullptr);
	//w.hCursor = LoadCursor(NULL, IDC_ARROW);

	////�E�B���h�E�N���X��OS�ɓo�^����
	//RegisterClassEx(&w);
	////�E�B���h�E�T�C�Y{X�� Y���@�����@�c��}
	//RECT wrc = { 0,0,window_width,window_height };
	////�����ŃT�C�Y�̕␳������
	//AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);


	////�E�B���h�E�I�u�W�F�N�g�̐���
	//HWND hwnd = CreateWindow(
	//	w.lpszClassName,		//�N���X��
	//	L"LE2B_07_�I�I�j�V_�q���L_GE3",			//�^�C�g���o�[�̕���
	//	WS_OVERLAPPEDWINDOW,	//�W���I�ȃE�B���h�E�X�^�C��
	//	CW_USEDEFAULT,			//�\��X���W(OS�ɔC����)
	//	CW_USEDEFAULT,			//�\��Y���W(OS�ɔC����)
	//	wrc.right - wrc.left,	//�E�B���h�E����
	//	wrc.bottom - wrc.top,	//�E�B���h�E�c��
	//	nullptr,				//�e�E�B���h�E�n���h��
	//	nullptr,				//���j���[�n���h��
	//	w.hInstance,			//�Ăяo���A�v���P�[�V�����n���h��
	//	nullptr);				//�I�v�V����

	////�E�B���h�E��\����Ԃɂ���
	//ShowWindow(hwnd, SW_SHOW);

	MSG msg{};	//���b�Z�[�W

	//-------DirectX�����������@��������-------//

//#ifdef _DEBUG
//	//�f�o�b�N���C���[���I����
//	ID3D12Debug* debugController;
//	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
//		debugController->EnableDebugLayer();
//	}
//
//#endif


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

	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	//IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	//ID3D12GraphicsCommandList* commandList = nullptr;
	//ID3D12CommandQueue* commandQueue = nullptr;
	//ID3D12DescriptorHeap* rtvHeap = nullptr;


	//�X�P�[�����O�{��
	XMFLOAT3 scale;

	//��]�p
	XMFLOAT3 rotation;

	//���W
	XMFLOAT3 position;

	scale = { 1.0f,1.0f,1.0f };

	rotation = { 0.0f,0.0f,0.0f };

	position = { 0.0f,0.0f,0.0f };

	float angle = 0.0f;


	//-------DirectX�����������@�����܂�-------//


	//���_�f�[�^
	Vertex vertices[] = {
		//  x      y      z      u    v
		//�O
		{{-5.0f, -5.0f, -5.0f},{}, {0.0f,1.0f}}, //����
		{{-5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}}, //����
		{{ 5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}}, //�E��
		{{ 5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}}, //�E��
		//���
		{{-5.0f, -5.0f,  5.0f},{0,0,1},{0.0f,1.0f}}, //����
		{{-5.0f,  5.0f,  5.0f},{0,0,1},{0.0f,0.0f}}, //����
		{{ 5.0f, -5.0f,  5.0f},{0,0,1},{1.0f,1.0f}}, //�E��
		{{ 5.0f,  5.0f,  5.0f},{0,0,1},{1.0f,0.0f}}, //�E��
		//��
		{{-5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}}, //����
		{{-5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}}, //����
		{{-5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}}, //�E��
		{{-5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}}, //�E��
		//�E
		{{ 5.0f, -5.0f, -5.0f},{1,0,0},{0.0f,1.0f}}, //����
		{{ 5.0f, -5.0f,  5.0f},{1,0,0},{0.0f,0.0f}}, //����
		{{ 5.0f,  5.0f, -5.0f},{1,0,0},{1.0f,1.0f}}, //�E��
		{{ 5.0f,  5.0f,  5.0f},{1,0,0},{1.0f,0.0f}}, //�E��

		{{-5.0f,  5.0f,  5.0f},{},{0.0f,1.0f}}, //����
		{{-5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}}, //����
		{{ 5.0f,  5.0f,  5.0f},{},{1.0f,1.0f}}, //�E��
		{{ 5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}}, //�E��
		//��
		{{-5.0f, -5.0f,  5.0f},{0,1,0},{0.0f,1.0f}}, //����
		{{-5.0f, -5.0f, -5.0f},{0,1,0},{0.0f,0.0f}}, //����
		{{ 5.0f, -5.0f,  5.0f},{0,1,0},{1.0f,1.0f}}, //�E��
		{{ 5.0f, -5.0f, -5.0f},{0,1,0},{1.0f,0.0f}}, //�E��
	};

	//�C���f�b�N�X�f�[�^
	unsigned short indices[] = {
		//�O
		0,1,2, //�O�p�`1��
		2,1,3, //�O�p�`2��
		//���
		5,4,6,
		7,5,6,
		//��
		8,9,10,
		10,9,11,
		//�E
		14,13,12,
		14,15,13,
		//��
		16,17,18,
		18,17,19,
		//��
		21,20,22,
		21,22,23,
	};

	//-------�`�揉�������� ��������-------//
	for (int i = 0; i < 23 / 3; i++) {
		//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ�������

	//   �����Ȃ�  int��蕝����������
		unsigned short indiceszero = indices[i * 3 + 0];
		unsigned short indicesone = indices[i * 3 + 1];
		unsigned short indicestwo = indices[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[indiceszero].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indicesone].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indicestwo].pos);
		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g�����Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[indiceszero].normal, normal);
		XMStoreFloat3(&vertices[indicesone].normal, normal);
		XMStoreFloat3(&vertices[indicestwo].normal, normal);
	}

	//���_�f�[�^�S�̂̃T�C�Y�@= ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; //�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	 //GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProp, //�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, //���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; //���W���R�s�[
	}
	//�q���������
	vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	//���_����̃f�[�^�̃T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	ID3D12Resource* constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff; //256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = dxCommon->GetDevice()->CreateCommittedResource
	(&cbHeapProp,
	 D3D12_HEAP_FLAG_NONE,
	 &cbResourceDesc,
	 D3D12_RESOURCE_STATE_GENERIC_READ,
	 nullptr,
	 IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); //�}�b�s���O
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̐���(����)
	ID3D12Resource* constBuffTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;

	ID3D12Resource* constBuffTransform1 = nullptr;
	ConstBufferDataTransform* constMapTransform1 = nullptr;

	{
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}

	//�萔�o�b�t�@�̐���
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&cbHeapProp,				//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,			//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform0));
	assert(SUCCEEDED(result));

	result = dxCommon->GetDevice()->CreateCommittedResource(
		&cbHeapProp,				//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,			//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform1));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0);	//�}�b�s���O
	assert(SUCCEEDED(result));

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1);	//�}�b�s���O
	assert(SUCCEEDED(result));

	//�l���������݂Ǝ����I�ɓ]�������
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);

	//�P�ʍs�����
	constMapTransform0->mat = XMMatrixIdentity();

	constMapTransform0->mat.r[0].m128_f32[0] = 2.0f / 1280;
	constMapTransform0->mat.r[1].m128_f32[1] = -2.0f / 720;
	constMapTransform0->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform0->mat.r[3].m128_f32[1] = 1.0f;

	constMapTransform0->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, WinApp::window_width,
		0.0f, WinApp::window_height,
		0.0f, 1.0f
	);

	//�ˉe�ϊ��s��(�������e)
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		(float)WinApp::window_width / WinApp::window_height,
		0.1f, 1000.0f
	);

	//�r���[�ϊ�
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -200);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//-------�摜�t�@�C���̗p��-------//
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resource/mario.jpg",
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	ScratchImage mipChain{};

	//�~�j�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t�[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);
	//-------�e�N�X�`���o�b�t�@�ݒ�-------//

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;	//��
	textureResourceDesc.Height = (UINT)metadata.height;	//����
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//-------�e�N�X�`���o�b�t�@�̐���-------//
	ID3D12Resource* texBuff = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource
	(&textureHeapProp,
	 D3D12_HEAP_FLAG_NONE,
	 &textureResourceDesc,
	 D3D12_RESOURCE_STATE_GENERIC_READ,
	 nullptr,
	 IID_PPV_ARGS(&texBuff));

	 //-------�e�N�X�`���o�b�t�@�̃f�[�^�]��-------//

	 //�S�~�b�v�}�b�v�ɂ���
	 for (size_t i = 0; i < metadata.mipLevels; i++) {
	 	//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
	 	const Image* img = scratchImg.GetImage(i, 0, 0);
	 	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	 	result = texBuff->WriteToSubresource(
	 		(UINT)i,
	 		nullptr,							//�S�̈�ւ̃R�s�[
	 		img->pixels,							//���f�[�^�A�h���X
	 		(UINT)img->rowPitch,	//1���C���T�C�Y
	 		(UINT)img->slicePitch	//�S�T�C�Y
	 	);
	 	assert(SUCCEEDED(result));
	 }


	 //-------�f�X�N���v�^�q�[�v����-------//
	 //SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = dxCommon->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//-------�V�F�[�_���\�[�X�r���[�̍쐬-------//
	//�V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};					//�ݒ�\����
	srvDesc.Format = resDesc.Format;			//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;		//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//�n���h���̂����ʒu�ɃV�F�[�_�[�肻���т�[�쐬
	dxCommon->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", //�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "vs_5_0", //�G���g���[�|�C���g���A�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�N�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	//�G���[�Ȃ�
	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					error.begin());
		error += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicPS.hlsl", //�V�F�[�_�t�@�C���p
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�N�p�ݒ�
		0,
		&psBlob, &errorBlob);

	//�G���[�Ȃ�
	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					error.begin());
		error += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		//xyz���W
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},	//(��s�ŏ������������₷��)
		//�@���x�N�g��
		{
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		//uv���W
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //�W���ݒ�

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBA�S�Ẵ`�����l����`��

	blenddesc.BlendEnable = true;						//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;		//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;		//�f�X�g�̒l��0%�g��

	////���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;				//�f�X�g�̒l��100%�g��

	////���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;				//�f�X�g�̒l��100%�g��

	////�F���]
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f-�f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;				//�g��Ȃ�

	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-�\�[�X�̃A���t�@�l

	//���X�^���C�U�̐ݒ�
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //�J�����O���Ȃ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; //�w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; //�|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; //�[�x�N���b�s���O��L����

	////�u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBA�S�Ẵ`�����l����`��

	//���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; //�`��Ώۂ͈��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; //1�s�N�Z���ɂ�1��T���v�����O

	//-------�f�X�N���v�^�����W�̐ݒ�-------//

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//�萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	//�e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_���猩����
	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;		//���[�g�p�����[�^�[�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);			//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
										 &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
										 IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature;

	//-------�f�v�X�X�e���V���X�e�[�g�̐ݒ�-------//
	pipelineDesc.DepthStencilState.DepthEnable = true;								//�[�x�e�X�g
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;		//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;			//��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;									//�[�x�l�t�H�[�}�b�g

	//�p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;
	result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; //�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProp,				//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,				//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);

	//�S�ẴC���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i]; //�C���f�b�N�X���R�s�[
	}

	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

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

		//���W���ړ����鏈��(Z���W)
		if (input->PushKey(DIK_UP)) {
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
		}


		//���[���h�ϊ�
		XMMATRIX matWorld0;

		//�X�P�[�����O�s��
		XMMATRIX matScale0;
		matScale0 = XMMatrixScaling(scale.x, scale.y, scale.z);

		//��]�s��
		XMMATRIX matRot0;
		matRot0 = XMMatrixIdentity();
		matRot0 *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));	//Z���܂���45�x��]
		matRot0 *= XMMatrixRotationX(XMConvertToRadians(rotation.x));	//X���܂���15�x��]
		matRot0 *= XMMatrixRotationY(XMConvertToRadians(rotation.y));	//Y���܂���30�x��]

		//���s�ړ��s��
		XMMATRIX matTrans0;
		matTrans0 = XMMatrixTranslation(position.x, position.y, position.z);

		matWorld0 = XMMatrixIdentity();
		matWorld0 *= matScale0;	//���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld0 *= matRot0;		//���[���h�s��ɉ�]�𔽉f
		matWorld0 *= matTrans0;


		constMapTransform0->mat = matWorld0 * matView * matProjection;	//-------�摜�C���[�W�f�[�^�̍쐬-------//

		//���[���h�ϊ�
		XMMATRIX matWorld1;

		//�X�P�[�����O�s��
		XMMATRIX matScale1;
		matScale1 = XMMatrixScaling(1.0f, 1.0f, 1.0f);

		//��]�s��
		XMMATRIX matRot1;
		matRot1 = XMMatrixIdentity();
		matRot1 *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));	//Z���܂���45�x��]
		matRot1 *= XMMatrixRotationX(XMConvertToRadians(rotation.x));	//X���܂���15�x��]
		matRot1 *= XMMatrixRotationY(XMConvertToRadians(XM_PI / 4.0f));	//Y���܂���30�x��]

		//���s�ړ��s��
		XMMATRIX matTrans1;
		matTrans1 = XMMatrixTranslation(-20.0f, 0, 0);

		matWorld1 = XMMatrixIdentity();
		matWorld1 *= matScale1;	//���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld1 *= matRot1;		//���[���h�s��ɉ�]�𔽉f
		matWorld1 *= matTrans1;


		constMapTransform1->mat = matWorld1 * matView * matProjection;	//-------�摜�C���[�W�f�[�^�̍쐬-------//

		if (input->PushKey(DIK_D) || input->PushKey(DIK_A)) {
			if (input->PushKey(DIK_D)) { angle += XMConvertToRadians(1.0f); }
			else if (input->PushKey(DIK_A)) { angle -= XMConvertToRadians(1.0f); }

			//angle���W�A������������ɉ�]�B���a��-100
			eye.x = -200 * (sinf(angle));
			eye.z = -200 * (cosf(angle));
			/*rotation.x = -200 * (sinf(angle));
			rotation.y = -200 * (cosf(angle));*/
			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		}

		//constMapTransform0->mat = matWorld0 * matView * matProjection;


		//�`��O����
		dxCommon->PreDraw();

		//-------4.�`��R�}���h��������-------//

		//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		dxCommon->GetCommandList()->SetPipelineState(pipelineState);
		dxCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature);
		//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
		dxCommon->GetCommandList()->IASetIndexBuffer(&ibView);

		//�v���~�e�B�u�`��̐ݒ�R�}���h
		dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //�O�p�`�̃��X�g


		//���_�o�b�t�@�r���[�̐ݒ�R�}���h
		dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

		//SRV�q�[�v�̐ݒ�R�}���h
		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		dxCommon->GetCommandList()->SetDescriptorHeaps(1, &srvHeap);
		//SRV�q�[�v�̐擪�n���h�����擾
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform0->GetGPUVirtualAddress());
		//�`��R�}���h
		dxCommon->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); //�S�Ă̒��_���g���ĕ`��


		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
		//�`��R�}���h
		dxCommon->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); //�S�Ă̒��_���g���ĕ`��

		////-------4.�`��R�}���h�����܂�-------//

		//�`��㏈��
		dxCommon->PostDraw();
		
		assert(SUCCEEDED(result));
		
		assert(SUCCEEDED(result));

		//-------DirectX���t���[�������@�����܂�-------//
	}

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