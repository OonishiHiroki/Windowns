//#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
#include<d3dcompiler.h>
#include<string>
#include<DirectXMath.h>
#include<DIrectXTex.h>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")

//定数バッファ用データ構造体
struct ConstBufferDataMaterial {
	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }; //色(RGBA)
};

//定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	XMMATRIX mat; //3D変換行列
};

//頂点データ構造体
struct Vertex {
	XMFLOAT3 pos;	//xyz座標
	XMFLOAT3 normal;//法線ベクトル
	XMFLOAT2 uv;	//uv座標
};

//ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//メッセージに応じてゲーム特有の処理を行う
	switch (msg) {
		//ウィンドウが破棄された
		case WM_DESTROY:
			//OSに対して、アプリの終了を伝える
			PostQuitMessage(0);
			return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

#pragma region WindowCreate
	////ウィンドウサイズ
	//const int window_width = 1280; //横幅
	//const int window_height = 720; //縦幅

	////ウィンドウクラスの設定
	//WNDCLASSEX w{};
	//w.cbSize = sizeof(WNDCLASSEX);
	//w.lpfnWndProc = (WNDPROC)WindowProc;
	//w.lpszClassName = L"DirectX";
	//w.hInstance = GetModuleHandle(nullptr);
	//w.hCursor = LoadCursor(NULL, IDC_ARROW);

	////ウィンドウクラスをOSに登録する
	//RegisterClassEx(&w);
	////ウィンドウサイズ{X軸 Y軸　横幅　縦幅}
	//RECT wrc = { 0,0,window_width,window_height };
	////自動でサイズの補正をする
	//AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);


	////ウィンドウオブジェクトの生成
	//HWND hwnd = CreateWindow(
	//	w.lpszClassName,		//クラス名
	//	L"LE2B_07_オオニシ_ヒロキ_GE3",			//タイトルバーの文字
	//	WS_OVERLAPPEDWINDOW,	//標準的なウィンドウスタイル
	//	CW_USEDEFAULT,			//表示X座標(OSに任せる)
	//	CW_USEDEFAULT,			//表示Y座標(OSに任せる)
	//	wrc.right - wrc.left,	//ウィンドウ横幅
	//	wrc.bottom - wrc.top,	//ウィンドウ縦幅
	//	nullptr,				//親ウィンドウハンドル
	//	nullptr,				//メニューハンドル
	//	w.hInstance,			//呼び出しアプリケーションハンドル
	//	nullptr);				//オプション

	////ウィンドウを表示状態にする
	//ShowWindow(hwnd, SW_SHOW);

	MSG msg{};	//メッセージ

	//-------DirectX初期化処理　ここから-------//

//#ifdef _DEBUG
//	//デバックレイヤーをオンに
//	ID3D12Debug* debugController;
//	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
//		debugController->EnableDebugLayer();
//	}
//
//#endif


	//ポインタ
	Input* input = nullptr;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;

	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//入力の初期化
	input = new Input();
	input->Initialize(winApp);

	//DirectXの初期化
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


	//スケーリング倍率
	XMFLOAT3 scale;

	//回転角
	XMFLOAT3 rotation;

	//座標
	XMFLOAT3 position;

	scale = { 1.0f,1.0f,1.0f };

	rotation = { 0.0f,0.0f,0.0f };

	position = { 0.0f,0.0f,0.0f };

	float angle = 0.0f;


	//-------DirectX初期化処理　ここまで-------//


	//頂点データ
	Vertex vertices[] = {
		//  x      y      z      u    v
		//前
		{{-5.0f, -5.0f, -5.0f},{}, {0.0f,1.0f}}, //左下
		{{-5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}}, //左上
		{{ 5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}}, //右下
		{{ 5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}}, //右上
		//後ろ
		{{-5.0f, -5.0f,  5.0f},{0,0,1},{0.0f,1.0f}}, //左下
		{{-5.0f,  5.0f,  5.0f},{0,0,1},{0.0f,0.0f}}, //左上
		{{ 5.0f, -5.0f,  5.0f},{0,0,1},{1.0f,1.0f}}, //右下
		{{ 5.0f,  5.0f,  5.0f},{0,0,1},{1.0f,0.0f}}, //右上
		//左
		{{-5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}}, //左下
		{{-5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}}, //左上
		{{-5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}}, //右下
		{{-5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}}, //右上
		//右
		{{ 5.0f, -5.0f, -5.0f},{1,0,0},{0.0f,1.0f}}, //左下
		{{ 5.0f, -5.0f,  5.0f},{1,0,0},{0.0f,0.0f}}, //左上
		{{ 5.0f,  5.0f, -5.0f},{1,0,0},{1.0f,1.0f}}, //右下
		{{ 5.0f,  5.0f,  5.0f},{1,0,0},{1.0f,0.0f}}, //右上

		{{-5.0f,  5.0f,  5.0f},{},{0.0f,1.0f}}, //左下
		{{-5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}}, //左上
		{{ 5.0f,  5.0f,  5.0f},{},{1.0f,1.0f}}, //右下
		{{ 5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}}, //右上
		//上
		{{-5.0f, -5.0f,  5.0f},{0,1,0},{0.0f,1.0f}}, //左下
		{{-5.0f, -5.0f, -5.0f},{0,1,0},{0.0f,0.0f}}, //左上
		{{ 5.0f, -5.0f,  5.0f},{0,1,0},{1.0f,1.0f}}, //右下
		{{ 5.0f, -5.0f, -5.0f},{0,1,0},{1.0f,0.0f}}, //右上
	};

	//インデックスデータ
	unsigned short indices[] = {
		//前
		0,1,2, //三角形1つ目
		2,1,3, //三角形2つ目
		//後ろ
		5,4,6,
		7,5,6,
		//左
		8,9,10,
		10,9,11,
		//右
		14,13,12,
		14,15,13,
		//上
		16,17,18,
		18,17,19,
		//下
		21,20,22,
		21,22,23,
	};

	//-------描画初期化処理 ここから-------//
	for (int i = 0; i < 23 / 3; i++) {
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数を入れる

	//   符号なし  intより幅が狭い整数
		unsigned short indiceszero = indices[i * 3 + 0];
		unsigned short indicesone = indices[i * 3 + 1];
		unsigned short indicestwo = indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[indiceszero].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indicesone].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indicestwo].pos);
		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトル減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化(長さを1にする)
		normal = XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[indiceszero].normal, normal);
		XMStoreFloat3(&vertices[indicesone].normal, normal);
		XMStoreFloat3(&vertices[indicestwo].normal, normal);
	}

	//頂点データ全体のサイズ　= 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; //ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	 //GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProp, //ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, //リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; //座標をコピー
	}
	//繋がりを解除
	vertBuff->Unmap(0, nullptr);

	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点一つ分のデータのサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	ID3D12Resource* constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff; //256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = dxCommon->GetDevice()->CreateCommittedResource
	(&cbHeapProp,
	 D3D12_HEAP_FLAG_NONE,
	 &cbResourceDesc,
	 D3D12_RESOURCE_STATE_GENERIC_READ,
	 nullptr,
	 IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); //マッピング
	assert(SUCCEEDED(result));

	//定数バッファの生成(準備)
	ID3D12Resource* constBuffTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;

	ID3D12Resource* constBuffTransform1 = nullptr;
	ConstBufferDataTransform* constMapTransform1 = nullptr;

	{
		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}

	//定数バッファの生成
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&cbHeapProp,				//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,			//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform0));
	assert(SUCCEEDED(result));

	result = dxCommon->GetDevice()->CreateCommittedResource(
		&cbHeapProp,				//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,			//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform1));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0);	//マッピング
	assert(SUCCEEDED(result));

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1);	//マッピング
	assert(SUCCEEDED(result));

	//値を書き込みと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);

	//単位行列を代入
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

	//射影変換行列(透視投影)
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		(float)WinApp::window_width / WinApp::window_height,
		0.1f, 1000.0f
	);

	//ビュー変換
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -200);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//-------画像ファイルの用意-------//
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resource/mario.jpg",
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	ScratchImage mipChain{};

	//ミニマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフーズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);
	//-------テクスチャバッファ設定-------//

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;	//幅
	textureResourceDesc.Height = (UINT)metadata.height;	//高さ
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//-------テクスチャバッファの生成-------//
	ID3D12Resource* texBuff = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource
	(&textureHeapProp,
	 D3D12_HEAP_FLAG_NONE,
	 &textureResourceDesc,
	 D3D12_RESOURCE_STATE_GENERIC_READ,
	 nullptr,
	 IID_PPV_ARGS(&texBuff));

	 //-------テクスチャバッファのデータ転送-------//

	 //全ミップマップについて
	 for (size_t i = 0; i < metadata.mipLevels; i++) {
	 	//ミップマップレベルを指定してイメージを取得
	 	const Image* img = scratchImg.GetImage(i, 0, 0);
	 	//テクスチャバッファにデータ転送
	 	result = texBuff->WriteToSubresource(
	 		(UINT)i,
	 		nullptr,							//全領域へのコピー
	 		img->pixels,							//元データアドレス
	 		(UINT)img->rowPitch,	//1ラインサイズ
	 		(UINT)img->slicePitch	//全サイズ
	 	);
	 	assert(SUCCEEDED(result));
	 }


	 //-------デスクリプタヒープ生成-------//
	 //SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定を元にSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = dxCommon->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//-------シェーダリソースビューの作成-------//
	//シェーダリソースビューの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};					//設定構造体
	srvDesc.Format = resDesc.Format;			//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;		//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルのさす位置にシェーダーりそすびゅー作成
	dxCommon->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", //シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "vs_5_0", //エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバック用設定
		0,
		&vsBlob, &errorBlob);

	//エラーなら
	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					error.begin());
		error += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicPS.hlsl", //シェーダファイル用
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "ps_5_0", //エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバック用設定
		0,
		&psBlob, &errorBlob);

	//エラーなら
	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					error.begin());
		error += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		//xyz座標
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},	//(一行で書いた方が見やすい)
		//法線ベクトル
		{
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		//uv座標
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBA全てのチャンネルを描画

	blenddesc.BlendEnable = true;						//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;		//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;		//デストの値を0%使う

	////加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;				//デストの値を100%使う

	////減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;				//デストの値を100%使う

	////色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;				//使わない

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-ソースのアルファ値

	//ラスタライザの設定
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //カリングしない
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; //背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; //ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; //深度クリッピングを有効に

	////ブレンドステート
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBA全てのチャンネルを描画

	//頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	pipelineDesc.NumRenderTargets = 1; //描画対象は一つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング

	//-------デスクリプタレンジの設定-------//

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//全てのシェーダから見える
	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ
	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	//テクスチャサンプラーの設定
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

	//ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;		//ルートパラメーターの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);			//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
										 &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
										 IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	//-------デプスステンシルステートの設定-------//
	pipelineDesc.DepthStencilState.DepthEnable = true;								//深度テスト
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;		//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;			//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;									//深度値フォーマット

	//パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; //インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProp,				//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,				//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);

	//全てのインデックスに対して
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i]; //インデックスをコピー
	}

	//マッピング解除
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//-------描画初期化処理　ここまで-------//

	//ゲームループ
	while (true) {
		//Windowのメッセージ処理
		if (winApp->ProcessMessage()) {
			//ゲームループを抜ける
			break;
		}
		//-------DirectX毎フレーム処理　ここから-------//

		input->Update();

		//数字の0キーが押されたら
		if (input->PushKey(DIK_0)) {
			OutputDebugStringA("Hit 0\n");
		}

		//if (input->PushKey(DIK_SPACE)) //スペースキーが押されたら
		//{
		//	clearColor[0] = { 0.5f };
		//	clearColor[1] = { 0.5f };
		//	clearColor[2] = { 0.5f };
		//	clearColor[3] = { 0.5f };
		//}

		//if (!input->PushKey(DIK_SPACE)) //スペースキーが押されたら
		//{
		//	clearColor[0] = { 0.1f };
		//	clearColor[1] = { 0.25f };
		//	clearColor[2] = { 0.5f };
		//	clearColor[3] = { 0.0f };
		//}

		//いずれかのキーを押していたら

		//座標を移動する処理(Z座標)
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


		//ワールド変換
		XMMATRIX matWorld0;

		//スケーリング行列
		XMMATRIX matScale0;
		matScale0 = XMMatrixScaling(scale.x, scale.y, scale.z);

		//回転行列
		XMMATRIX matRot0;
		matRot0 = XMMatrixIdentity();
		matRot0 *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));	//Z軸まわりに45度回転
		matRot0 *= XMMatrixRotationX(XMConvertToRadians(rotation.x));	//X軸まわりに15度回転
		matRot0 *= XMMatrixRotationY(XMConvertToRadians(rotation.y));	//Y軸まわりに30度回転

		//平行移動行列
		XMMATRIX matTrans0;
		matTrans0 = XMMatrixTranslation(position.x, position.y, position.z);

		matWorld0 = XMMatrixIdentity();
		matWorld0 *= matScale0;	//ワールド行列にスケーリングを反映
		matWorld0 *= matRot0;		//ワールド行列に回転を反映
		matWorld0 *= matTrans0;


		constMapTransform0->mat = matWorld0 * matView * matProjection;	//-------画像イメージデータの作成-------//

		//ワールド変換
		XMMATRIX matWorld1;

		//スケーリング行列
		XMMATRIX matScale1;
		matScale1 = XMMatrixScaling(1.0f, 1.0f, 1.0f);

		//回転行列
		XMMATRIX matRot1;
		matRot1 = XMMatrixIdentity();
		matRot1 *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));	//Z軸まわりに45度回転
		matRot1 *= XMMatrixRotationX(XMConvertToRadians(rotation.x));	//X軸まわりに15度回転
		matRot1 *= XMMatrixRotationY(XMConvertToRadians(XM_PI / 4.0f));	//Y軸まわりに30度回転

		//平行移動行列
		XMMATRIX matTrans1;
		matTrans1 = XMMatrixTranslation(-20.0f, 0, 0);

		matWorld1 = XMMatrixIdentity();
		matWorld1 *= matScale1;	//ワールド行列にスケーリングを反映
		matWorld1 *= matRot1;		//ワールド行列に回転を反映
		matWorld1 *= matTrans1;


		constMapTransform1->mat = matWorld1 * matView * matProjection;	//-------画像イメージデータの作成-------//

		if (input->PushKey(DIK_D) || input->PushKey(DIK_A)) {
			if (input->PushKey(DIK_D)) { angle += XMConvertToRadians(1.0f); }
			else if (input->PushKey(DIK_A)) { angle -= XMConvertToRadians(1.0f); }

			//angleラジアンだけ軸周りに回転。半径は-100
			eye.x = -200 * (sinf(angle));
			eye.z = -200 * (cosf(angle));
			/*rotation.x = -200 * (sinf(angle));
			rotation.y = -200 * (cosf(angle));*/
			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		}

		//constMapTransform0->mat = matWorld0 * matView * matProjection;


		//描画前処理
		dxCommon->PreDraw();

		//-------4.描画コマンドここから-------//

		//パイプラインステートとルートシグネチャの設定コマンド
		dxCommon->GetCommandList()->SetPipelineState(pipelineState);
		dxCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature);
		//インデックスバッファビューの設定コマンド
		dxCommon->GetCommandList()->IASetIndexBuffer(&ibView);

		//プリミティブ形状の設定コマンド
		dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //三角形のリスト


		//頂点バッファビューの設定コマンド
		dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

		//SRVヒープの設定コマンド
		//定数バッファビュー(CBV)の設定コマンド
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		dxCommon->GetCommandList()->SetDescriptorHeaps(1, &srvHeap);
		//SRVヒープの先頭ハンドルを取得
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		//定数バッファビュー(CBV)の設定コマンド
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform0->GetGPUVirtualAddress());
		//描画コマンド
		dxCommon->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); //全ての頂点を使って描画


		//定数バッファビュー(CBV)の設定コマンド
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
		//描画コマンド
		dxCommon->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0); //全ての頂点を使って描画

		////-------4.描画コマンドここまで-------//

		//描画後処理
		dxCommon->PostDraw();
		
		assert(SUCCEEDED(result));
		
		assert(SUCCEEDED(result));

		//-------DirectX毎フレーム処理　ここまで-------//
	}

	//WindowAPIの終了処理
	winApp->Finalize();

	//各種解放
	delete input;
	delete winApp;
	delete dxCommon;

	////ウィンドウクラスを登録解除
	//UnregisterClass(w.lpszClassName, w.hInstance);


	//delete[] scratchImg;

	return 0;
}