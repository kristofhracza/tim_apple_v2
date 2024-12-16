#pragma once

#include <string>
#include <tuple>

#include "../../utilities/Vectors.h"




namespace ESP {
	void MainLoop();

	void BoundingBox(Vector3 vOrigin, ViewMatrix_t vViewMatrix);
	void Skeleton(Matrix2x4_t* pBoneCache);
	void HealthBar(int32_t iHealth);
	void PlayerName(std::string szName);
	void SnapLines();
	void Distance();
	void WeaponName(std::string szName, Vector3 vOrigin, bool IsOnGround = false);

	inline namespace Util {
		std::string GunIcon(std::string szName);
	}
}