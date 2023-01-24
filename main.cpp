//#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

#pragma region WindowCreate //基盤システムの初期化

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

		//いずれかのキーを押していたら

		//座標を移動する処理(Z座標)
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

		//描画前処理
		dxCommon->PreDraw();

		//-------4.描画コマンドここから-------//

		//描画後処理
		dxCommon->PostDraw();

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