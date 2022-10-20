#include "WinApp.h"
#include <Windows.h>


//Windowsアプリでのエントリーポイント(main関数)


LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//メッセージで分岐
	switch (msg) {
		case WM_DESTROY: //ウィンドウが破壊された
			PostQuitMessage(0); //osに対してアプリの終了を伝える
			return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void WinApp::Initialize() {
	// ウィンドウ横幅
	//const int window_width = 1280;
	// ウィンドウ縦幅
	//const int window_height = 720;


	//WNDCLASSEX w = {};
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.lpfnWndProc = (WNDPROC)WindowProc;	//ウィンドウプロシージャを設定
	winClass.lpszClassName = L"DX12Sample";			//ウィンドウクラス名
	winClass.hInstance = GetModuleHandle(nullptr);		//ウィンドウハンドル
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//カーソル指定

	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&winClass);
	//ウィンドウサイズ{X座標　Y座標　横幅　縦幅}
	RECT wrc = { 0,0,window_width,window_height };
	//関数を使ってウィンドウのサイズを自動で補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(winClass.lpszClassName,//クラス名指定
						L"LE2B_07_オオニシ_ヒロキ_GE3",					//タイトルバーの文字
						WS_OVERLAPPEDWINDOW,			//タイトルバーと境界線があるウィンドウ
						CW_USEDEFAULT,					//表示x座標はOSにお任せ
						CW_USEDEFAULT,					//表示y座標はOSにお任せ
						wrc.right - wrc.left,			//ウィンドウ幅
						wrc.bottom - wrc.top,			//ウィンドウ高
						nullptr,						//親ウィンドウハンドル
						nullptr,						//メニューハンドル
						winClass.hInstance,					//呼び出しアプリケーションハンドル
						nullptr);						//追加パラメーター(オプション)

					//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

void WinApp::Update() {

}

void WinApp::Finalize() {
	//ウィンドウクラスを登録解除
	UnregisterClass(winClass.lpszClassName, winClass.hInstance);
}

bool WinApp::ProcessMessage() {

	MSG msg{};

	//メッセージがある？
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);	//キー入力メッセージの処理
		DispatchMessage(&msg);	//プロシージャにメッセージを送る
	}

	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT) {
		return true;
	}
	return false;
}
