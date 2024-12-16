#pragma once
#pragma warning (disable: 4172)

#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

#include <numbers>
#include <cmath>

#include "../dependencies/imgui/imgui.h"



struct ViewMatrix_t{
	float* operator[ ](int nIndex) {
		return mMatrix[nIndex];
	}
	float mMatrix[4][4];
};


struct Vector2 {
	float x, y;
};


struct Vector3 {
	constexpr Vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }

	constexpr const Vector3& operator-(const Vector3& vOther) const noexcept
	{
		return Vector3{ x - vOther.x, y - vOther.y, z - vOther.z };
	}

	constexpr const Vector3& operator+(const Vector3& vOther) const noexcept
	{
		return Vector3{ x + vOther.x, y + vOther.y, z + vOther.z };
	}

	constexpr const Vector3& operator/(const float fFactor) const noexcept
	{
		return Vector3{ x / fFactor, y / fFactor, z / fFactor };
	}

	constexpr const Vector3& operator*(const float fFactor) const noexcept
	{
		return Vector3{ x * fFactor, y * fFactor, z * fFactor };
	}

	constexpr const Vector3& ToAngle() const noexcept
	{
		return Vector3{
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f 
		};
	}


	Vector3 WorldToScreen(ViewMatrix_t mMatrix) {
		float _x = mMatrix[0][0] * x + mMatrix[0][1] * y + mMatrix[0][2] * z + mMatrix[0][3];
		float _y = mMatrix[1][0] * x + mMatrix[1][1] * y + mMatrix[1][2] * z + mMatrix[1][3];

		float w = mMatrix[3][0] * x + mMatrix[3][1] * y + mMatrix[3][2] * z + mMatrix[3][3];

		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float screen_x = ImGui::GetIO().DisplaySize.x * 0.5f;
		float screen_y = ImGui::GetIO().DisplaySize.y * 0.5f;

		screen_x += 0.5f * _x * ImGui::GetIO().DisplaySize.x + 0.5f;
		screen_y -= 0.5f * _y * ImGui::GetIO().DisplaySize.y + 0.5f;

		return { screen_x, screen_y, w };
	}

	constexpr const bool IsZero() const noexcept{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	float x, y, z;
};


struct Vector4D_t{
	constexpr Vector4D_t(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f, const float w = 0.0f) :
		x(x), y(y), z(z), w(w) { }

	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
};


struct alignas(16) VectorAligned_t : Vector3{
	VectorAligned_t() = default;

	explicit VectorAligned_t(const Vector3& VecBase){
		this->x = VecBase.x;
		this->y = VecBase.y;
		this->z = VecBase.z;
		this->w = 0.0f;
	}

	constexpr VectorAligned_t& operator=(const Vector3& VecBase){
		this->x = VecBase.x;
		this->y = VecBase.y;
		this->z = VecBase.z;
		this->w = 0.0f;
		return *this;
	}

	float w = 0.0f;
};


struct Matrix2x4_t
{
public:

	[[nodiscard]] const Vector3 GetOrigin(int nIndex)
	{
		return Vector3(this[nIndex]._11, this[nIndex]._12, this[nIndex]._13);
	}

	const void SetOrigin(int nIndex, Vector3 VecValue)
	{
		this[nIndex]._11 = VecValue.x;
		this[nIndex]._12 = VecValue.y;
		this[nIndex]._13 = VecValue.z;
	}
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
		};
	};
};


struct C_UTL_VECTOR {
	int count = 0;
	int pad = 0;
	Vector3* data;
};

inline constexpr Vector3 CalculateAngle(
	const Vector3& vLocalPos,
	const Vector3& vEnemyPos,
	const Vector3& vWiewAngles) noexcept{
	return ((vEnemyPos - vLocalPos).ToAngle() - vWiewAngles);
};


inline Vector3 ClampAngles(Vector3 vAngles) {
	if (vAngles.x > 89.0f && vAngles.x <= 180.0f)
		vAngles.x = 89.0f;

	if (vAngles.x > 180.0f)
		vAngles.x -= 360.0f;

	if (vAngles.x < -89.0f)
		vAngles.x = -89.0f;

	if (vAngles.y > 180.0f)
		vAngles.y -= 360.0f;

	if (vAngles.y < -180.0f)
		vAngles.y += 360.0f;
	
	vAngles.z = 0;

	return vAngles;
};


inline Vector3 NormalizeAngles(Vector3 vAngles) {
	while (vAngles.x > 180.f)
		vAngles.x -= 360.0f;

	while (vAngles.x < -180.0f)
		vAngles.x += 360.0f;

	while (vAngles.y > 180.0f)
		vAngles.y -= 360.0f;

	while (vAngles.y < -180.0f)
		vAngles.y += 360.0f;

	return vAngles;
};


inline Vector3 CalculateBestAngle(Vector3 vAngles,float fConfigFov) {
	Vector3 vNewAngle;

	float fCalcFov = std::hypot(vAngles.x, vAngles.y);
	float fov = fConfigFov;

	if (fCalcFov < fov) {
		fov = fCalcFov;
		vNewAngle = vAngles;
	}
	return vNewAngle;
}

constexpr float Deg2Rad(const float Degree) noexcept{
	return Degree * (std::numbers::pi_v<float> / 180.f);
}