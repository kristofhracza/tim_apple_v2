#pragma once

#include <array>

#include "../../utilities/Virtual.hpp"
#include "../../utilities/Vectors.h"
#include "../../utilities/Scanner.hpp"



struct Ray_t{
public:
	Vector3 m_vecStart;
	Vector3 m_vecEnd;
	Vector3 m_vecMins;
	Vector3 m_vecMaxs;
	MEM_PAD(0x4);
	std::uint8_t UnkType;
};

struct SurfaceData_t{
public:
	MEM_PAD(0x8)
	float m_flPenetrationModifier;
	float m_flDamageModifier;
	MEM_PAD(0x4)
	int m_iMaterial;
};

struct TraceHitboxData_t{
public:
	MEM_PAD(0x38);
	int m_nHitGroup;
	MEM_PAD(0x4);
	int m_nHitboxId;
};

class C_CSPlayerPawn;

struct GameTrace_t{
public:
	GameTrace_t() = default;

	SurfaceData_t* GetSurfaceData();
	int GetHitboxId();
	int GetHitgroup();
	bool IsVisible() const;

	void* m_pSurface;
	C_CSPlayerPawn* m_pHitEntity;
	TraceHitboxData_t* m_pHitboxData;
	MEM_PAD(0x38);
	std::uint32_t m_uContents;
	MEM_PAD(0x24);
	Vector3 m_vecStartPos;
	Vector3 m_vecEndPos;
	Vector3 m_vecNormal;
	Vector3 m_vecPosition;
	MEM_PAD(0x4);
	float m_flFraction;
	MEM_PAD(0x6);
	bool m_bAllSolid;
	MEM_PAD(0x4D)
}; // Size: 0x108


struct TraceFilter_t{
public:
	MEM_PAD(0x8);
	std::int64_t m_uTraceMask;
	std::array<std::int64_t, 2> m_v1;
	std::array<std::int32_t, 4> m_arrSkipHandles;
	std::array<std::int16_t, 2> m_arrCollisions;
	std::int16_t m_v2;
	std::uint8_t m_v3;
	std::uint8_t m_v4;
	std::uint8_t m_v5;

	TraceFilter_t() = default;
	TraceFilter_t(std::uint64_t uMask, C_CSPlayerPawn* pSkip1, C_CSPlayerPawn* pSkip2, int nLayer);
};

class CGameTraceManager {
public:
	bool TraceShape(Ray_t* pRay, Vector3 vecStart, Vector3 vecEnd, TraceFilter_t* pFilter, GameTrace_t* pGameTrace){
		using fnTraceShape = bool(__fastcall*)(CGameTraceManager*, Ray_t*, Vector3*, Vector3*, TraceFilter_t*, GameTrace_t*);
		static fnTraceShape oTraceShape = reinterpret_cast<fnTraceShape>(Scanner::GetAbsoluteAddress(Scanner::PatternScan("client.dll", "E8 ? ? ? ? 80 7D 57 00"), 0x1, 0x0));
		return oTraceShape(this, pRay, &vecStart, &vecEnd, pFilter, pGameTrace);
	}

	bool ClipRayToEntity(Ray_t* pRay, Vector3 vecStart, Vector3 vecEnd, C_CSPlayerPawn* pPawn, TraceFilter_t* pFilter, GameTrace_t* pGameTrace){
		using fnClipRayToEntity = bool(__fastcall*)(CGameTraceManager*, Ray_t*, Vector3*, Vector3*, C_CSPlayerPawn*, TraceFilter_t*, GameTrace_t*);
		static fnClipRayToEntity oClipRayToEntity = reinterpret_cast<fnClipRayToEntity>(Scanner::PatternScan("client.dll", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0 00 00 00 48 8B 9C"));
		return oClipRayToEntity(this, pRay, &vecStart, &vecEnd, pPawn, pFilter, pGameTrace);
	}
};