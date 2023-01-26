#pragma once
#include "DirectXCommon.h"

class Sprite {
public:
	//‰Šú‰»
	void Initialize(DirectXCommon *dxCommon);

	//•`‰æ
	void Draw();
private:
	DirectXCommon* dxCommon = nullptr;
};

