#pragma once

#include "interfaces/Interfaces.hpp"
#include "interfaces/EntityHandle.h"

#include "schemasystem/Schema.hpp"

#include "../utilities/Vectors.h"
#include "../../utilities/Logger.hpp"



// Not a big fan of forward decl. but good for now
class CEntityInstance;
class CSkeletonInstance;
class CGlowProperty;
class CPlayer_ItemServices;
class CPlayer_CameraServices;
class CCSPlayer_ViewModelServices;
class CCSPlayer_WeaponServices;
class C_CSWeaponBase;
class C_EconItemView;
class CPlayer_WeaponServices;


// Entity related classes
class CEntityIdentity {
public:
	SCHEMA("CEntityIdentity","m_designerName",GetDesignerName,const char*);
	SCHEMA("CEntityIdentity", "m_flags", GetFlags, std::uint32_t);
	SCHEMA_OFFSET("CEntityIdentity", GetIndex, std::uint32_t, 0x10);

	[[nodiscard]] bool IsValid(){
		return GetIndex() != INVALID_EHANDLE_INDEX;
	}

	[[nodiscard]] int GetEntryIndex(){
		if (!IsValid())
			return ENT_ENTRY_MASK;

		return GetIndex() & ENT_ENTRY_MASK;
	}

	[[nodiscard]] int GetSerialNumber(){
		return GetIndex() >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	CEntityInstance* m_pInstance;
};


class CEntityInstance {
public:
	void GetSchemaClassInfo(SchemaClassInfoData_t** m_pReturn){
		return Virtual::Call<void, 38>(this, m_pReturn);
	}

	[[nodiscard]] CBaseHandle GetRefEHandle(){
		CEntityIdentity* pIdentity = GetIdentity();
		if (pIdentity == nullptr)
			return CBaseHandle();

		return CBaseHandle(pIdentity->GetEntryIndex(), pIdentity->GetSerialNumber() - (pIdentity->GetFlags() & 1));
	}

	SCHEMA("CEntityInstance", "m_pEntity", GetIdentity, CEntityIdentity*);
};

class CCollisionProperty{
public:
	std::uint16_t CollisionMask(){
		return *reinterpret_cast<std::uint16_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x38);
	}

	SCHEMA("CCollisionProperty", "m_vecMaxs", GetMaxs, Vector3);
	SCHEMA("CCollisionProperty", "m_CollisionGroup", GetCollisionGroup, std::uint8_t);
	SCHEMA("CCollisionProperty", "m_usSolidFlags", GetSolidFlags, std::uint8_t);
};

class CGameSceneNode {
public:
	CSkeletonInstance* GetSkeletonInstance(){
		return Virtual::Call<CSkeletonInstance*, 8>(this);
	}

	SCHEMA("CGameSceneNode", "m_pOwner", GetOwner, CEntityInstance*);
	SCHEMA("CGameSceneNode", "m_vecAbsOrigin", GetAbsOrigin, Vector3);
	SCHEMA("CGameSceneNode", "m_vRenderOrigin", GetRenderOrigin, Vector3);
	SCHEMA("CGameSceneNode", "m_bDormant", IsDormant, bool);

};

class C_BaseEntity : public CEntityInstance {
public:
	static C_BaseEntity* GetLocalPlayer();
	[[nodiscard]] const Vector3& GetSceneOrigin();

	bool IsWeapon();

	SCHEMA("C_BaseEntity", "m_iHealth", GetHealth, std::int32_t);
	SCHEMA("C_BaseEntity", "m_iMaxHealth", GetMaxHealth, std::int32_t);
	SCHEMA("C_BaseEntity", "m_iTeamNum", GetTeam, std::int32_t);
	SCHEMA("C_BaseEntity", "m_fFlags", GetFlags, std::uint32_t);
	SCHEMA("C_BaseEntity", "m_lifeState", GetLifeState, std::uint8_t);
	SCHEMA("C_BaseEntity", "m_flWaterLevel", GetWaterLevel, float);
	SCHEMA("C_BaseEntity", "m_bTakesDamage", IsTakingDamage, bool);
	SCHEMA("C_BaseEntity", "m_hOwnerEntity", GetOwner, CBaseHandle);
	SCHEMA("C_BaseEntity", "m_pGameSceneNode", GetGameSceneNode, CGameSceneNode*);
	SCHEMA("C_BaseEntity", "m_hOwnerEntity", GetOwnerHandle, CBaseHandle);
	SCHEMA("C_BaseEntity", "m_pCollision", GetCollision, CCollisionProperty*);
	SCHEMA_OFFSET_PROPERTY("C_BaseEntity", "m_nSubclassID", GetSubClassID, void*, 0x8);
};


class C_BaseModelEntity : public C_BaseEntity {
public:
	SCHEMA("C_BaseModelEntity", "m_vecViewOffset", GetVecViewOffset, Vector3);
	SCHEMA("C_BaseModelEntity", "m_Glow", GetGlow, CGlowProperty);
};


class C_BasePlayerPawn : public C_BaseModelEntity {
public:
	[[nodiscard]] Vector3 GetEyePosition();

	SCHEMA("C_BasePlayerPawn", "m_hController", GetControllerHandle, CBaseHandle);
	SCHEMA("C_BasePlayerPawn", "m_pWeaponServices", GetWeaponServices, CPlayer_WeaponServices*);
	SCHEMA("C_BasePlayerPawn", "m_pItemServices", GetItemServices, CPlayer_ItemServices*);
	SCHEMA("C_BasePlayerPawn", "m_pCameraServices", GetCameraServices, CPlayer_CameraServices*);
	SCHEMA("C_BasePlayerPawn", "m_vOldOrigin", GetOrigin, Vector3);
};

class C_CSPlayerPawnBase : public C_BasePlayerPawn {
public:
	SCHEMA("C_CSPlayerPawnBase", "m_flFlashDuration", GetFlashDuration , float);
	SCHEMA("C_CSPlayerPawnBase", "m_flFlashMaxAlpha", GetFlashMaxAlpha, float);
	SCHEMA("C_CSPlayerPawnBase", "m_pViewModelServices", GetViewModelServices, CCSPlayer_ViewModelServices*);
	SCHEMA("C_CSPlayerPawnBase", "m_pClippingWeapon", GetClippingWeapon, C_CSWeaponBase*);
};


class C_CSPlayerPawn : public C_CSPlayerPawnBase {
public:
	[[nodiscard]] std::uint32_t GetOwnerHandleIndex();
	[[nodiscard]] std::uint16_t GetCollisionMask();

	SCHEMA("C_CSPlayerPawn", "m_bIsScoped", IsScoped, bool);
	SCHEMA("C_CSPlayerPawn", "m_bIsDefusing", IsDefusing, bool);
	SCHEMA("C_CSPlayerPawn", "m_iShotsFired", GetShotsFired, int);
	SCHEMA("C_CSPlayerPawn", "m_ArmorValue", GetArmorValue, std::int32_t);
	SCHEMA("C_CSPlayerPawn", "m_aimPunchAngle", GetAimPunchAngle, Vector3);
	SCHEMA("C_CSPlayerPawn", "m_aimPunchCache", GetAimPunchCache, C_UTL_VECTOR);
};


class CBasePlayerController : public C_BaseModelEntity {
public:
	SCHEMA("CBasePlayerController", "m_steamID", GetSteamID, std::uint64_t);
	SCHEMA("CBasePlayerController", "m_bIsLocalPlayerController", IsLocalPlayerController, bool);
	SCHEMA("CBasePlayerController", "m_hPawn", GetPawnHandle, CBaseHandle);
};


class CCSPlayerController : public CBasePlayerController {
public:
	[[nodiscard]] static CCSPlayerController* GetLocalPlayerController();
	[[nodiscard]] const Vector3& GetPawnOrigin();

	SCHEMA("CCSPlayerController", "m_iPing", GetPing, std::uint32_t);
	SCHEMA("CCSPlayerController", "m_iPawnHealth", GetPawnHealth, std::int32_t);
	SCHEMA("CCSPlayerController", "m_sSanitizedPlayerName", GetPlayerName, const char *);
	SCHEMA("CCSPlayerController", "m_bPawnIsAlive", IsPawnAlive, bool);
	SCHEMA("CCSPlayerController", "m_bPawnHasDefuser", IsPawnHasDefuser, bool);
	SCHEMA("CCSPlayerController", "m_hPlayerPawn", GetPlayerPawnHandle, CBaseHandle);

};



// Item related classes
class CBaseAnimGraph : public C_BaseModelEntity{
public:
};


class C_BaseFlex : public CBaseAnimGraph {
public:
};


class CAttributeManager{
public:
	virtual ~CAttributeManager() = 0;
};


class C_AttributeContainer : public CAttributeManager {
public:
	SCHEMA("C_AttributeContainer", "m_Item", GetItem, C_EconItemView);
};


class C_EconEntity : public C_BaseFlex {
public:
	SCHEMA("C_EconEntity", "m_AttributeManager", GetAttributeManager, C_AttributeContainer);
};


class C_EconItemView {
public:
	SCHEMA("C_EconItemView", "m_szCustomName", GetCustomName, const char*);
	SCHEMA("C_EconItemView", "m_szCustomNameOverride", GetCustomNameOverride, const char*);
	SCHEMA("C_EconItemView", "m_iItemID", GetItemID, std::uint32_t);
	SCHEMA("C_EconItemView", "m_iItemDefinitionIndex", GetItemDefinitionIndex, std::uint16_t);
};


class CPlayer_WeaponServices{
public:
	SCHEMA("CPlayer_WeaponServices", "m_hActiveWeapon", GetActiveWeapon, CBaseHandle);
};


class CCSPlayer_WeaponServices : public CPlayer_WeaponServices{
public:
	SCHEMA("CCSPlayer_WeaponServices", "m_flNextAttack", GetNextAttack, std::float_t);
};


class CBasePlayerWeaponVData {
public:
};


class CCSWeaponBaseVData : public CBasePlayerWeaponVData {
public:
	SCHEMA("CCSWeaponBaseVData", "m_bIsFullAuto", IsFullAuto, bool);
};


class C_BasePlayerWeapon : public C_EconEntity{
public:
	SCHEMA("C_BasePlayerWeapon", "m_nNextPrimaryAttackTick", GetNextPrimaryAttackTick, std::int32_t);
};


class C_CSWeaponBase : public C_BasePlayerWeapon {
public:
	SCHEMA("C_CSWeaponBase", "m_bInReload", IsInReload, bool);

	CCSWeaponBaseVData* GetWeaponVData(){
		return static_cast<CCSWeaponBaseVData*>(GetSubClassID());
	}
};


class C_SmokeGrenadeProjectile{
public:
	SCHEMA("C_SmokeGrenadeProjectile","m_vSmokeColor", GetColour, Vector3);
};


// Skeleton
class CSkeletonInstance : public CGameSceneNode{
public:
	MEM_PAD(0x1CC);
	int nBoneCount;
	MEM_PAD(0x18);
	int nMask;
	MEM_PAD(0x4);
	Matrix2x4_t* pBoneCache;
};
