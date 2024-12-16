#pragma once

#include <string>
#include <vector>
#include <tuple>

#include "Vectors.h"

#pragma warning (disable: 4244)



inline namespace Helpers {
	// This is from some random guy, I'm not good at math
	inline float GetDistance(Vector3 vFrom, Vector3 vTo) {
		return static_cast<float>(sqrt(pow(vTo.x - vFrom.x, 2) + pow(vTo.y - vFrom.y, 2) + pow(vTo.z - vFrom.z, 2)));
	}

	// Fix functions allow texts to adjust to player distances
	// So they'll look relatively the same

	inline float FixFontSize(float fSize) {
		float nReturnSize = 1.f;

		if (fSize > 4.f) nReturnSize = 4.f;
		if (fSize < 1.f) nReturnSize = 1.f;

		return nReturnSize;
	}

	inline float FixJointSize(float fSize) { // Assume font size is 5!!!
		float nReturnSize = 1.f;

		if (fSize > 3.f) nReturnSize = 3.f;
		if (fSize > 7.f) nReturnSize = 4.f;
		if (fSize < 1.f) nReturnSize = 2.f;

		return nReturnSize;
	}

	inline float GetFontSize(float fFontSize, int iDistance) {
		return (fFontSize - FixFontSize(iDistance));
	}

	inline float GetJointSize(float joinSize, int iDistance) {
		return (joinSize - FixJointSize(iDistance));
	}

	inline std::tuple<float, float> GetTextOffsets(float x, float y, float fHorizontalDivide, float fVerticalDivide = 1) {
		float fHorizontalOffset = x / fHorizontalDivide;
		float fVerticalOffset = y - fVerticalDivide;

		std::tuple<float, float> tCoords = { fHorizontalOffset, fVerticalOffset };
		return tCoords;
	}
};