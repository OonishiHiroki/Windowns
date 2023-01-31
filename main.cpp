//#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"

//定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color;//色(RGBA)
	XMMATRIX mat; //3D変換行列
};

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

#pragma region WindowCreate

	MSG msg{};	//メッセージ

	//-------DirectX初期化処理　ここから-------//

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


	//-------DirectX初期化処理　ここまで-------//


	//-------描画初期化処理　ここまで-------//

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

		////座標を移動する処理(Z座標)
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

		//	//angleラジアンだけ軸周りに回転。半径は-100
		//	eye.x = -200 * (sinf(angle));
		//	eye.z = -200 * (cosf(angle));
		//	/*rotation.x = -200 * (sinf(angle));
		//	rotation.y = -200 * (cosf(angle));*/
		//	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		//}

		//constMapTransform0->mat = matWorld0 * matView * matProjection;

		//描画前処理
		dxCommon->PreDraw();

		//-------4.描画コマンドここから-------//
		sprite->SpriteCommonBeginDraw(dxCommon->GetCommandList(), spriteCommon_);
		title.SpriteDraw(dxCommon->GetCommandList(), spriteCommon_, dxCommon->GetDevice(), title.vbView);
		////-------4.描画コマンドここまで-------//

		//描画後処理
		dxCommon->PostDraw();

		//-------DirectX毎フレーム処理　ここまで-------//
	}

	delete sprite;

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