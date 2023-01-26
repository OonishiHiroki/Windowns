#pragma once
#include "DirectXCommon.h"

class SpriteCommon {
public:

	//‰Šú‰»
	void Initialize(DirectXCommon *dxCommon);

	//•`‰æ
	void Draw();

private:
	DirectXCommon* dxCommon = nullptr;
	
};

