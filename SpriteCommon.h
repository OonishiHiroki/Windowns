#pragma once
#include "DirectXCommon.h"

class SpriteCommon {
public:

	//������
	void Initialize(DirectXCommon *dxCommon);

	//�`��
	void Draw();

private:
	DirectXCommon* dxCommon = nullptr;
	
};

