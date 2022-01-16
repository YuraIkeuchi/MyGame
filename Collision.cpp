#include "Collision.h"

#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

bool Collision::SphereCollision(const float& X1, const float& Y1, const float& Z1,const float& R1, const float& X2, const float& Y2, const float& Z2, const float& R2) {
	int a = X1 - X2;
	int b = Y1 - Y2;
	int c = Z1 - Z2;
	int distance = sqrtf(a * a + b * b + c * c);
	int radius = R1 + R2;
	if (distance <= radius) {
		return true;
	} else {
		return false;
	}

	return true;
}

