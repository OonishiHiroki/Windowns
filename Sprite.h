#pragma once
#include "DirectXCommon.h"

class Sprite {
public:
	//������
	void Initialize(DirectXCommon *dxCommon);

	//�`��
	void Draw();
private:
	DirectXCommon* dxCommon = nullptr;
};

